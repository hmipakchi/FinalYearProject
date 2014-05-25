function [groups,Q,numiter] = klnB(groups,B,verbose)
%
% Newman's Kernighan-Lin-inspired iterative improvement scheme, described in
% "Modularity and community structure in networks," PNAS 103, 8577-8582 (2006).
%
%   [groups,numiter] = klnB(groups,B)
%   [groups,numiter] = klnB(groups,B,0)
%
% Reassigns nodes (only those listed in indx, if specified) between the 
% groups [groups(indx)] partitions according to Newman's recipe, repeating 
% iteratively (through numiter counted iterations) until no further 
% improvement in modularity is found.
%
% -PJM 4 March 2008: modified 5 June to directly take in B instead of A.
% -Inderjit Jutla, July 2011: rewrote some part of the code to improve 
%        perfomance, overall code still operates in the exact same way. 
%
% Setting the third argument to 0 suppresses displayed text output.

if (nargin<3),
    verbose=1;
end
if verbose
    mydisp = @(s) disp(s);
else
    mydisp = @(s) disp('');
end

if (size(groups,1)>size(groups,2)),
    groups=groups';
end

%Identify distinct group values and number of cut levels in dendrogram:
groupnumbers=unique(groups);
differences=diff(groupnumbers);
cuts=length(unique(differences));

%Identify partitions to iterate with:
indx=1:length(B);
gpart=unique(groups(indx));

%Some basics about the network:
N=length(B);

%Initial modularity:
Q=modularity(groups,B);
indxorig=indx;
numiter=0;
Q0=-99999999999;
%Iterations:

while (Q>Q0), %Outer loop until improvement not found inside
    mydisp([int2str(numiter),':  Q = ',num2str(Q)])
    Q0=Q;
    numiter=numiter+1;
    indx=indxorig;
    testgroups=groups;
    while ~isempty(indx), %Move each node precisely once
        clocktime=clock;
        %mydisp([num2str(length(indx)),' nodes to move ',num2str(clocktime(4:6))])
        nodecontributions=zeros(length(B),length(gpart));
        %nn = zeros(length(gpart),length(B));
        %for ig=1:length(gpart)
        %    nn(ig,:) = (testgroups==gpart(ig));
        %end
        %nodecontributions = B*(nn'); 
        nn = zeros(length(B),length(gpart));
        for ig=1:length(gpart)
           nn(:,ig) = (testgroups==gpart(ig))';
        end
        nodecontributions = B*(nn); 
        
        glookup(gpart)=1:length(gpart);
        currcontribindex=[1:N]'+(glookup(testgroups)'-1)*N;
        currcontrib=nodecontributions(currcontribindex)+diag(B);
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
        indx((indx==besti))=[];
        %remove i from indx list of nodes to be moved
        bestQ=modularity(testgroups,B); %calculate Q
        if (bestQ>Q), %Keep track of configuration with highest Q.
            groups=testgroups;
            Q=bestQ;
        end
    end
end

mydisp([int2str(numiter),':  Stopped because bestQ = ',num2str(bestQ)])
Q=modularity(groups,B);
end
%----------------------------%
function Q = modularity(g,B)
numcomms=max(g);
P = zeros(numcomms,length(B));
Q=0;
for ic=1:numcomms,
    P(ic,:) = (g==ic);
end
Q = sum(sum((P*B).*P));
end

%Q=Q/sum(sum(A));

%----------------------------%
function v = BMultiply(x,B)
v=B*x;
end
%j=full(sum(A,2)); %j is a column vector
%jx=j'*x; %jx is the dot product of j and x
%twom=sum(j);
%jx=full(jx);
%x2=(A*x);
%x3=(j*jx)/twom;
%v=x2-gamma*x3;

