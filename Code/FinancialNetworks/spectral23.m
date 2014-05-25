function [gg,Q,counts] = spectral23(A,Bin,flags,Q3factor)
% SPECTRAL23: Spectral partitioning by modularity eigenvectors, including
% 1-vector and 2-vector bisection, and two-vector three-way division, with
% available KL iterative improvement and extended subpartitioning, as
% described in Richardson, T., Mucha, P.J. & Porter, M.A., Spectral
% tripartitioning of networks, Phys. Rev. E 80, 036111-10 (2009).
%
% S = spectral23(B)     [assumes B is a matrix]
% S = spectral23(A,B)
% [S,Q] = spectral23(A,B)
% [S,Q,counts] = spectral23(A,B)
% [S,Q,counts] = spectral23(A,B,flags)
% [S,Q,counts] = spectral23(A,B,flags,Q3factor)
%
% A: Adjacency matrix
% B: Modularity matrix (for your selected null model)
% flags: vector of at most four logicals
%       flags(1): include KL postprocessing (on by default)
%       flags(2): include extended subdivision (off by default)
%       flags(3): include two-vector bipartitioning steps (on by default)
%       flags(4): include three-vector tripartitioning steps (on by default)
% Q3factor (default = 1.05): minimum gain factor needed in employed
%       divide-and-conquer tripartioning strategy
%
% The most efficient use of this code is through specification of B(v,A,IX)
% functions for calculating B*v, requiring in the implementation used here
% that both A and B be specified. If A is specified and B is a positive
% scalar, that value is used as the multiplicative resolution parameter on
% the Newman-Girvan null model. If A is specified and B is a negative
% scalar, it's magnitude is used as the probability value in the uniform
% null model.
%
% Use of extended subpartitioning requires the B(v,A,IX) function include a
% global BPASSGENFLAG which is initially declared = 1 for regular
% partitioning and then set = 0 for extended subpartitioning (see the use
% in BNG and Buniform here, as examples; sorry it's such a hack!).
%
% NOTE THAT THERE ARE MANY WAYS THIS CODE COULD BE IMPROVED.
% WE WERE MERELY TRYING TO GET IT WORKING AS A PROOF OF PRINCIPLE.
%
% Peter J. Mucha, October 2009
%

global BPASSGENFLAG PASSQ3FACTOR
BPASSGENFLAG=1;

%Establish B(v) function returning B(g)*v for arbitrary v vector
if nargin==1
    Bin=A;
    B = @(v,A,IX) (Bin(IX,IX)-diag(sum(Bin(IX,IX))))*v;
elseif exist('Bin','var')==0
    B = @(v,A,IX) BNG(v,A,IX,1); %if Bin is not specified
elseif isempty(Bin)
    B = @(v,A,IX) BNG(v,A,IX,1); %if Bin is not specified
elseif isa(Bin,'function_handle')
    B = Bin;
elseif max(size(Bin))==1
    if Bin>0
        B = @(v,A,IX) BNG(v,A,IX,Bin); %if Bin is a positive scaler
    else
        B = @(v,A,IX) Buniform(v,A,IX,-Bin); %if Bin is a negative scaler
    end
elseif size(Bin,1)==size(Bin,2) %assuming Bin is a matrix, ignore A
    B = @(v,A,IX) (Bin(IX,IX)-diag(sum(Bin(IX,IX))))*v;
else
    disp('Problem determing how to work with B')
    return
end

%Set defaults:
klnflag=1; %include KL(N) postprocessing by default
extsubflag=0; %skip extended subdivision by default
flag2=1; %include two-vector two-way divisions by default
flag3=1; %include three-way divisions by default
if exist('flags','var'),
    if ~isempty(flags),
        klnflag=flags(1);
        if length(flags)>1,
            extsubflag=flags(2);
            if length(flags)>2,
                flag2=flags(3);
                if length(flags)>3,
                    flag3=flags(4);
                end
            end
        end
    end
end
if exist('Q3factor','var')==0
    Q3factor=1.05; %Continue 3-way divide-and-conquer on sufficient improvement
end
PASSQ3FACTOR=Q3factor;

%Strip off A diagonal just to be safe:
A=A-diag(diag(A));

%Start with every node in the same group
n=size(A,1);
g=ones(1,n); %g is our list of group numbers
%S=sparse(1:n,g,1); %S is the community assignment matrix (do we need it?)

%Subdividing until no further improvement in modularity:
counts=zeros(1,3);
opts.disp=0; opts.issym=1; opts.isreal=1;
tosplit=true;
while any(tosplit),
    ug=unique(g);
    for ig=ug(tosplit(ug)),
        
        indx=find(g==ig);
        ng=length(indx);
        c=zeros(3,ng);
        
        %Get and sort eigenvectors:
        if ng>2
            [V,D,FLAG] = eigs(B,ng,3,'be',opts,A,indx);
            if FLAG, disp('WARNING: EIGS did not converge!'); end
            D=diag(D); [D,iord]=sort(D,'descend'); V=V(:,iord);
        else
            [V,D,FLAG] = eigs(B,ng,2,'la',opts,A,indx);
            if FLAG, disp('WARNING: EIGS did not converge!'); end
            D=diag(D); [D,iord]=sort(D,'descend'); V=V(:,iord);
            D(3)=min(0,D(2)-1); %This is a hack to make negative eigenvalues work
        end
            
        %Leading eigenvector (set 0 to -1 group; will KLN later):
        C=sign(V(:,1)); C(C~=1)=2; c(1,:)=C';
        dQ(1) = modularity(c(1,:),B,A,indx);
        
        %convert eigenvectors to vertex vectors and
        %identify distinct cut angles in the plane:
        d = sqrt(D(1:2)-D(3))';
        V = V(:,1:2).*d(ones(ng,1),:);

        %Leading pair bisection:
        if (flag2)
            c(2,:) = twoway2(V,B,A,indx);
            dQ(2) = modularity(c(2,:),B,A,indx);
        end
        
        %Leading pair trichotomy:
        if (flag3)
            c(3,:) = threeway2(V,B,A,indx);
            dQ(3) = modularity(c(3,:),B,A,indx);
        end
        
        %disp(['dQ: ',num2str(dQ)])
        
        %Identify best of three methods and proceed appropriately:
        [bestdQ,ib]=max(dQ);
        if (bestdQ>0)&&(max(c(ib,:))-min(c(ib,:))>0)
            counts(ib)=counts(ib)+1;
            g1=(c(ib,:)==1);
            g2=(c(ib,:)==2);
            g3=(c(ib,:)==3);
            maxg=max(g);
            g(indx(g2))=maxg+1;
            g(indx(g3))=maxg+2;
            tosplit(ig)=(nnz(g1)>1);
            tosplit(maxg+1)=(nnz(g2)>1);
            tosplit(maxg+2)=(nnz(g3)>1);
        else
            tosplit(ig)=false;
        end

    end
end
disp('Finished recursive spectral partitioning to improve modularity')
gg=g;

%Subdividing until no further improvement in "local modularity":
%Note, we could possibly check our modularity calls by making sure that the
%difference between the full generalized modularity and local modularity
%results are shifted correctly. Note also that one could in principle
%continue to subdivide using the best dQ available, even if it is <0, until
%every node partitioned into its own community.
%Note also that this coding is really ugly here; the local modularity loop
%below is almost identical to the generalized modularity loop above.
ug=unique(g);
for ig=ug,
    tosplit(ig)=(nnz(g==ig)>1);
end
BPASSGENFLAG=0;
if extsubflag
    while any(tosplit),
        ug=unique(g);
        for ig=ug(tosplit(ug)),

            indx=find(g==ig);
            ng=length(indx);
            c=zeros(3,ng);
        
            Alocal=A(indx,indx);
            indlocal=1:ng;

            %Get and sort eigenvectors:
            if ng>2
                [V,D,FLAG] = eigs(B,ng,3,'be',opts,Alocal,indlocal);
                if FLAG, disp('WARNING: EIGS did not converge!'); end
                D=diag(D); [D,iord]=sort(D,'descend'); V=V(:,iord);
            else
                [V,D,FLAG] = eigs(B,ng,2,'la',opts,Alocal,indlocal);
                if FLAG, disp('WARNING: EIGS did not converge!'); end
                D=diag(D); [D,iord]=sort(D,'descend'); V=V(:,iord);
                D(3)=min(0,D(2)-1); %This is a hack to make negative eigenvalues work
            end        

            %Leading eigenvector (set 0 to -1 group; will KLN later):
            C=sign(V(:,1)); C(C~=1)=2; c(1,:)=C';
            dQ(1) = modularity(c(1,:),B,Alocal,indlocal);

            %convert eigenvectors to vertex vectors:
            d = sqrt(D(1:2)-D(3))';
            V = V(:,1:2).*d(ones(ng,1),:);

            %Leading pair bisection:
            if (flag2)
                c(2,:) = twoway2(V,B,Alocal,indlocal);
                dQ(2) = modularity(c(2,:),B,Alocal,indlocal);
            end

            %Leading pair trichotomy:
            if (flag3)
                c(3,:) = threeway2(V,B,Alocal,indlocal);
                dQ(3) = modularity(c(3,:),B,Alocal,indlocal);
            end

            disp(['dQ: ',num2str(dQ)])

            %Identify best of three methods and proceed appropriately:
            [bestdQ,ib]=max(dQ);
            if bestdQ>0&&(max(c(ib,:))-min(c(ib,:))>0)
                counts(ib)=counts(ib)+1;
                g1=(c(ib,:)==1);
                g2=(c(ib,:)==2);
                g3=(c(ib,:)==3);
                maxg=max(g);
                g(indx(g2))=maxg+1;
                g(indx(g3))=maxg+2;
                tosplit(ig)=(nnz(g1)>1);
                tosplit(maxg+1)=(nnz(g2)>1);
                tosplit(maxg+2)=(nnz(g3)>1);
            else
                tosplit(ig)=false;
            end

        end
    end
    gg=[gg;g];
    disp('Finished recursive extended subpartitioning')
end

%KLN iterations to improve result:
if klnflag
    g=kln(g,B,A);
    gg=[gg;g];
end

%Cleanup group numbers, get modularity:
for i=1:size(gg,1),
    g=gg(i,:);
    ug=unique(g); 
    for j=1:length(ug), 
        g(g==ug(j))=j; 
    end
    gg(i,:)=g;
    Q(i)=modularity(g,B,A,1:n);
end
ng=max(gg')
%twom=sum(sum(A));
%Q=Q/twom;
%end spectral23 main%


%-----%
function pass = twoway2(V,B,A,indx)
[pthetas,rdummy]=cart2pol(V(:,1),V(:,2));
temp=sort(mod(pthetas,pi));
cutthetas=temp-min(diff([0;temp]))/2;
ng=length(indx);
QQ=zeros(1,ng);
for icut=1:ng, %overkill, could be improved
    C=ones(ng,1);
    relangle=mod(pthetas-cutthetas(icut)+pi,2*pi)-pi;
    C(relangle<0)=2;
    QQ(icut)=modularity(C',B,A,indx); %overkill, could be improved
end
[Q2,maxcut]=max(QQ);
C2=ones(1,ng);
relangle=mod(pthetas-cutthetas(maxcut)+pi,2*pi)-pi;
C2(relangle<0)=2;
pass=C2;
%end twoway2%


%-----%
function pass = threeway2(V,B,A,indx)
global PASSQ3FACTOR
disp(['threeway2: length(indx)=',int2str(length(indx))])
maxwedges=min(2^6,length(indx));
Q0=-999;
numcoarsewedges=4;
[Q,C]=threewaycoarse(V,B,A,indx,numcoarsewedges);
QQ=Q;
while (Q>Q0+(PASSQ3FACTOR-1)*abs(Q0))&&(numcoarsewedges*2<=maxwedges)
    Q0=Q;
    numcoarsewedges=numcoarsewedges*2;
    [Q,C]=threewaycoarse(V,B,A,indx,numcoarsewedges);
    QQ=[QQ,Q];
    disp(num2str(QQ))
end
%disp(['threeway2: ',num2str(QQ)])
pass=C;
%end threeway2%


%-----%
function [Q3,C3] = threewaycoarse(V,B,A,indx,numcoarsewedges)
%Set up coarse wedge boundary thetas
[pthetas,rdummy]=cart2pol(V(:,1),V(:,2));
N = length(indx);
cutthetas=(-1:2)'*pi/2;
if numcoarsewedges>4
    %stds=[1,1]; %No rescaling; pure octants
    stds = std(V); %Scaling corners by standard deviations
    cornerangle=cart2pol(stds(1),stds(2));
    numper8=numcoarsewedges/8;
    quadrant=[linspace(0,cornerangle,numper8+1),...
        linspace(cornerangle,pi/2,numper8+1)]';
    angles=[quadrant;pi-quadrant;-quadrant;-pi+quadrant];
    angles=sort(unique(angles));
    cutthetas=angles(2:end);
end
Ncuts=length(cutthetas);
Q3=-999;
C3=ones(1,N);
size3 = (Ncuts*(Ncuts-1)*(Ncuts-2))/6;
QQ = zeros(1,size3);
count = 1;
for i1=1:(Ncuts-2) %down
	for i2=(i1+1):(Ncuts-1) %the
		for i3=(i2+1):Ncuts     %tiny rabbit hole 
            C=ones(1,N);
            C((pthetas>cutthetas(i1))&(pthetas<=cutthetas(i2)))=2;
            C((pthetas>cutthetas(i2))&(pthetas<=cutthetas(i3)))=3;
            Q=modularity(C,B,A,indx);
			QQ(count) = Q;
			count = count+1;
			if Q > Q3
                %[cutthetas(i1),cutthetas(i2),cutthetas(i3),V(1,1),V(1,2)]
                C3 = C;
				Q3 = Q;
			end
		end
	end
end
count=count-1;
%end threewaycoarse%


%-----%
function [groups,Q,numiter] = kln(groups,B,A)
%modified from klnslow; need klnfast modifications for large networks

%Identify distinct group values and number of cut levels in dendrogram:
groupnumbers=unique(groups);
differences=diff(groupnumbers);
cuts=length(unique(differences));

%Some basics about the network:
N=length(groups);
Bdiag=zeros(N,1);
for i=1:N,
    v=zeros(N,1);
    v(i)=1;
    v=B(v,A,1:N);
    Bdiag(i)=v(i);
end

%Initial modularity:
Q=modularity(groups,B,A,1:N);
numiter=0;
Q0=-999;
%Iterations:
while (Q>Q0), %Outer loop until improvement not found inside
    disp([int2str(numiter),':  Q = ',num2str(Q)])
    Q0=Q;
    numiter=numiter+1;
    indx=1:N;
    testgroups=groups;
    while ~isempty(indx), %Move each node precisely once
        gpart=unique(testgroups);
        nodecontributions=zeros(length(groups),length(gpart));
        for ig=1:length(gpart),
            nodecontributions(:,ig)=B((testgroups==gpart(ig))',A,1:N);
        end
        glookup(gpart)=1:length(gpart);
        currcontribindex=(1:N)'+(glookup(testgroups)'-1)*N;
        currcontrib=nodecontributions(currcontribindex)-Bdiag;
        nodecontributions=nodecontributions-...
            currcontrib*ones(1,length(gpart));
        nodecontributions(currcontribindex)=-999; %Ignore current placement
        nodecontributions=nodecontributions(indx,:); %Restrict to available nodes
        maxvalue=max(max(nodecontributions)); %Identify best available switch
        [i,g]=find(nodecontributions==maxvalue);
        ii=ceil(length(i)*rand(1)); %break ties randomly
        besti=indx(i(ii)); bestg=gpart(g(ii));
        testgroups(besti)=bestg; %move node i to new group
        %besti,bestg,testgroups,indx
        indx(indx==besti)=[]; %remove i from indx list of nodes to be moved
        bestQ=modularity(testgroups,B,A,1:N); %calculate Q
        if (bestQ>Q), %Keep track of configuration with highest Q.
            groups=testgroups;
            Q=bestQ;
        end
    end
end
disp([int2str(numiter),':  Stopped because bestQ = ',num2str(bestQ)])
Q=modularity(groups,B,A,1:N);
%end kln%


%-----%
function Q = modularity(g,B,A,IX)
Q=0;
for ic=unique(g),
    x=(g==ic);
    Q=Q+x*B(x',A,IX);
end
%end modularity%


%-----%
function pass = BNG(v,A,IX,gamma)
%Pre-multiplication of general column vector v by the Newman-Girvan
%generalized modularity matrix on indices IX
global BPASSGENFLAG
k=sum(A);
vBsum = v.*sum(A(IX,IX),2) - v.*k(IX)'*gamma*sum(k(IX))/sum(k);
pass = A(IX,IX)*v - gamma*k(IX)'*(k(IX)*v)/sum(k) - BPASSGENFLAG*vBsum;
%end BNG%


%-----%
function pass = Buniform(v,A,IX,gamma)
%Pre-multiplication of general column vector v by the random uniform
%generalized modularity matrix on indices IX
global BPASSGENFLAG
vBsum = v.*sum(A(IX,IX),2) - v*gamma*length(IX);
pass = A(IX,IX)*v - gamma*v - BPASSGENFLAG*vBsum;
%end Buniform%
