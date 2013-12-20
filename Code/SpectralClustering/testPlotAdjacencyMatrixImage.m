clear;

% definition of the stochastic block model

% number vertices
n = 1000;

% number of communities
q = 3;
    
% probability matrix indicating the proability of edge between communities
pout = 0.02;
pin = 0.04;

cin = floor(pin*n);
cout = floor(pout*n);

affinityMatrix = (pout .* ones(q,q)) + ((pin-pout) .* eye(q,q));

% choose communitiy for each edge (i.e. 1,...,q)
nodeCommunities = zeros(n);
for i=1:n
    nodeCommunitity = mod((i-1), q) + 1;
    nodeCommunities(i) = nodeCommunitity;
end

% evaluate number of nodes per community
numberNodesPerCommunity = zeros(q,1);
for i=1:n
    nodeCommunitity = nodeCommunities(i);
    numberNodesPerCommunity(nodeCommunitity) = numberNodesPerCommunity(nodeCommunitity) + 1;
end

% evaluate cumulative number of nodes up to a community
cumulativeNumberNodesUptoCommunity = zeros(q,1);
for i=1:q
    if i >= 2
        cumulativeNumberNodesUptoCommunity(i) = cumulativeNumberNodesUptoCommunity(i-1) + numberNodesPerCommunity(i-1);
    end
end

% evaluate all node members for each community
communityNodeMemberships = -1.*ones(q,max(numberNodesPerCommunity));
communityNodeMembershipsIterators = ones(q,1);
for i=1:n
    nodeCommunitity = nodeCommunities(i);
    communityNodeMemberships(nodeCommunitity,communityNodeMembershipsIterators(nodeCommunitity)) = i;
    communityNodeMembershipsIterators(nodeCommunitity) = communityNodeMembershipsIterators(nodeCommunitity) + 1;
end

% create adjacency matrix for this stochastic block model
adjacencyMatrix = zeros(n,n);
for i=1:n
    for j=1:n
        % no self edges allowed
        if i ~= j
            flipResult = rand(1);
            if flipResult <= affinityMatrix(nodeCommunities(i),nodeCommunities(j))
                adjacencyMatrix(i,j) = 1;
            end
        end
    end
end

% labelled adjacency matrix for this stochastic block model to demonstrate
% communities
labelledAdjacencyMatrix = zeros(n,n);
for c1=1:q
    for c2=1:q
        for i=1:numberNodesPerCommunity(c1)
            for j=1:numberNodesPerCommunity(c2)
                rowValueAdjMatrix = communityNodeMemberships(c1,i);
                colValueAdjMatrix = communityNodeMemberships(c2,j);
                rowValueLabelledAdjMatrix = cumulativeNumberNodesUptoCommunity(c2) + j;
                colValueLabelledAdjMatrix = cumulativeNumberNodesUptoCommunity(c1) + i;
                labelledAdjacencyMatrix(rowValueLabelledAdjMatrix,colValueLabelledAdjMatrix) = adjacencyMatrix(rowValueAdjMatrix,colValueAdjMatrix);
            end
        end
    end
end

% write Adjacency Matrix to file
fileID = fopen('data_files/adjacencyMatrices/adjacencyMatrix.dat','w');
for i=n:-1:1
    for j=1:n
        if j ~= n
            fprintf(fileID,'%d ',adjacencyMatrix(i,j));
        else
            fprintf(fileID,'%d',adjacencyMatrix(i,j));
        end
    end
    fprintf(fileID,'\n');
end
fclose(fileID);

% write Labelled Adjacency Matrix to file
fileID = fopen('data_files/adjacencyMatrices/labelledAdjacencyMatrix.dat','w');
for i=n:-1:1
    for j=1:n
        if j ~= n
            fprintf(fileID,'%d ',labelledAdjacencyMatrix(i,j));
        else
            fprintf(fileID,'%d',labelledAdjacencyMatrix(i,j));
        end
    end
    fprintf(fileID,'\n');
end
fclose(fileID);