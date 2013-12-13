clear;

% definition of the stochastic block model

% number vertices
n = 100;
% number of communities
q = 4;
    
% probability matrix indicating the proability of edge between communities
pout = 0.5;
pin = 1.0;

cin = floor(pin*n);
cout = floor(pout*n);

affinityMatrix = (pout .* ones(q,q)) + ((pin-pout) .* eye(q,q));

% choose communitiy for each edge (i.e. 1,...,q)
nodeCommunities = zeros(n, 1);
numberMembersCommunities = zeros(q,1);
cumulativeMembersCommunities = zeros(q,1);
for i=1:n
    nodeCommunities(i,1) = mod((i-1), q) + 1;
    numberMembersCommunities(mod((i-1), q) + 1, 1) = numberMembersCommunities(mod((i-1), q) + 1, 1) + 1;
end
for j = 2:q
    cumulativeMembersCommunities(j,1) = numberMembersCommunities(j-1,1) + cumulativeMembersCommunities(j-1,1);
end

% create adjacency matrix for this stochastic block model
adjacencyMatrix = zeros(n,n);
for i=1:n
    for j=1:n
        if i ~= j
            flipResult = rand(1);
            if flipResult <= affinityMatrix(nodeCommunities(i,1),nodeCommunities(j,1))
                adjacencyMatrix(i,j) = 1;
            end
        end
    end
end

labelledAdjacencyMatrix = zeros(n,n);
orderedIndicesI = zeros(n,1);
orderedIndicesJ = zeros(n,1);
for i=1:n
    for j=1:n
        nodeICommunity = nodeCommunities(i,1);
        nodeJCommunity = nodeCommunities(j,1);
        
        orderedIndexI = cumulativeMembersCommunities(nodeICommunity,1) + ((i-nodeICommunity)/q) + 1;
        orderedIndexJ = cumulativeMembersCommunities(nodeJCommunity,1) + ((j-nodeJCommunity)/q) + 1;
        
        labelledAdjacencyMatrix(orderedIndexI,orderedIndexJ) = adjacencyMatrix(i,j);
    end
end

subplot(1, 2, 1);
pcolor(adjacencyMatrix);
colormap([1 1 1; 0 0 0;]);
axis ij;
axis square;

subplot(1, 2, 2);
pcolor(labelledAdjacencyMatrix);
colormap([1 1 1; 0 0 0;]);
axis ij;
axis square;