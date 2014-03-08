clear;

% definition of the stochastic block model

% number vertices
hcmN = 150;%300

% number of communities
hcmQ = 1;
    
% probability matrix indicating the probability of edge between communities
hcmPout = 0.1;%0.3
hcmPin = 1.0;%0.7

hcmCin = floor(hcmPin*hcmN);
hcmCout = floor(hcmPout*hcmN);

alteredEyeMatrix = eye(hcmQ+1,hcmQ+1);
alteredEyeMatrix(hcmQ+1,hcmQ+1) = 0;
hcmAffinityMatrix = (hcmPout .* ones(hcmQ+1,hcmQ+1)) + ((hcmPin-hcmPout) .* alteredEyeMatrix);
whichModel_str = 'hcm';

% choose number of nodes per community
hcmNumberNodesPerCommunity = [30;120];

% evaluate cumulative number of nodes up to a community
hcmCumulativeNumberNodesUptoCommunity = zeros(hcmQ+1,1);
for i=1:hcmQ+1
    if i >= 2
        hcmCumulativeNumberNodesUptoCommunity(i) = hcmCumulativeNumberNodesUptoCommunity(i-1) + hcmNumberNodesPerCommunity(i-1);
    end
end

% choose communitiy for each edge
hcmNodeCommunities = zeros(hcmN,1);
tempCumulativeCommunityCount = zeros(hcmQ+1,1);
hitNumberNodesLimit = zeros(hcmQ+1,1);
nodeCommunity = 1;
temp = 1;
for i=1:hcmN
    tooManyNodes = 1;
    while (tooManyNodes == 1)
        flipResult = rand(1);
        for j=1:hcmQ+1
            if hitNumberNodesLimit(j) == 0
                bottomProbability = (hcmCumulativeNumberNodesUptoCommunity(j)/hcmN);
            else
                bottomProbability = 1;
            end
            if j ~= hcmQ+1
                topProbability = (hcmCumulativeNumberNodesUptoCommunity(j+1)/hcmN);
                if (flipResult <= topProbability) && (flipResult >= bottomProbability)
                    nodeCommunity = j;
                end
            else
                if flipResult >= bottomProbability
                    nodeCommunity = j;
                end
            end
        end
        if (tempCumulativeCommunityCount(nodeCommunity) < hcmNumberNodesPerCommunity(nodeCommunity))
            tempCumulativeCommunityCount(nodeCommunity) = tempCumulativeCommunityCount(nodeCommunity) + 1;
            hcmNodeCommunities(i) = nodeCommunity;
            tooManyNodes = 0;
        else
            hitNumberNodesLimit(nodeCommunity) = 1;
            tooManyNodes = 1;
        end
    end
%     if temp < hcmQ+1
%         if i <= hcmCumulativeNumberNodesUptoCommunity(temp+1)
%             nodeCommunity = temp;
%         else
%             temp = temp + 1;
%             nodeCommunity = temp;
%         end
%     else
%         nodeCommunity = temp;
%     end
%     hcmNodeCommunities(i) = nodeCommunity;
end

% evaluate all node members for each community
hcmCommunityNodeMemberships = -1.*ones(hcmQ+1,max(hcmNumberNodesPerCommunity));
hcmCommunityNodeMembershipsIterators = ones(hcmQ+1,1);
for i=1:hcmN
    nodeCommunitity = hcmNodeCommunities(i);
    if hcmCommunityNodeMembershipsIterators(nodeCommunitity) <= max(hcmNumberNodesPerCommunity)
        hcmCommunityNodeMemberships(nodeCommunitity,hcmCommunityNodeMembershipsIterators(nodeCommunitity)) = i;
        hcmCommunityNodeMembershipsIterators(nodeCommunitity) = hcmCommunityNodeMembershipsIterators(nodeCommunitity) + 1;
    end
end

% create adjacency matrix for this stochastic block model
hcmAdjacencyMatrix = zeros(hcmN,hcmN);
for i=1:hcmN
    for j=1:hcmN
        % no self edges allowed
        if i ~= j
            flipResult = rand(1);
            if flipResult <= hcmAffinityMatrix(hcmNodeCommunities(i),hcmNodeCommunities(j))
                hcmAdjacencyMatrix(i,j) = 1;
            end
        end
    end
end

% labelled adjacency matrix for this stochastic block model to demonstrate
% communities
hcmLabelledAdjacencyMatrix = zeros(hcmN,hcmN);
for c1=1:hcmQ+1
    for c2=1:hcmQ+1
        for i=1:hcmNumberNodesPerCommunity(c1)
            for j=1:hcmNumberNodesPerCommunity(c2)
                rowValueAdjMatrix = hcmCommunityNodeMemberships(c1,i);
                colValueAdjMatrix = hcmCommunityNodeMemberships(c2,j);
                rowValueLabelledAdjMatrix = hcmCumulativeNumberNodesUptoCommunity(c2) + j;
                colValueLabelledAdjMatrix = hcmCumulativeNumberNodesUptoCommunity(c1) + i;
                hcmLabelledAdjacencyMatrix(rowValueLabelledAdjMatrix,colValueLabelledAdjMatrix) = hcmAdjacencyMatrix(rowValueAdjMatrix,colValueAdjMatrix);
            end
        end
    end
end

% write Adjacency Matrix to file
adjacencyMatrixFilename = sprintf('data_files/adjacencyMatrices/%sAdjacencyMatrix.dat',whichModel_str);
fileID = fopen(adjacencyMatrixFilename,'w');
for i=hcmN:-1:1
    for j=1:hcmN
        if j ~= hcmN
            fprintf(fileID,'%d ',hcmAdjacencyMatrix(i,j));
        else
            fprintf(fileID,'%d',hcmAdjacencyMatrix(i,j));
        end
    end
    fprintf(fileID,'\n');
end
fclose(fileID);

% write Labelled Adjacency Matrix to file
% labelledAdjacencyMatrixFilename = sprintf('data_files/adjacencyMatrices/%sLabelledAdjacencyMatrix.dat',whichModel_str);
% fileID = fopen(labelledAdjacencyMatrixFilename,'w');
% for i=hcmN:-1:1
%     for j=1:hcmN
%         if j ~= hcmN
%             fprintf(fileID,'%d ',hcmAdjacencyMatrix(i,j));
%         else
%             fprintf(fileID,'%d',hcmAdjacencyMatrix(i,j));
%         end
%     end
%     fprintf(fileID,'\n');
% end
% fclose(fileID);

% write Adjacency Matrix to gexf file
adjacencyMatrixGEXFFilename = sprintf('data_files/adjacencyMatrices/%sExampleGraph.gexf',whichModel_str);
fileID = fopen(adjacencyMatrixGEXFFilename,'w');
fprintf(fileID,'<gexf version="1.1"><meta><creator>Hesam Ipakchi</creator></meta><graph defaultedgetype="directed" idtype="string" type="static">');
fprintf(fileID,'<nodes count="%d">',hcmN);
for i=1:hcmN
    fprintf(fileID,'<node id="%d" label="%d"/>',i,i);
end
fprintf(fileID,'</nodes>');
hcmNodeDegrees = sum(hcmAdjacencyMatrix);
hcmTotalNoEdges = sum(hcmNodeDegrees);
fprintf(fileID,'<edges count="%d">',hcmTotalNoEdges);
edgesCount = 1;
for i=1:hcmN
    for j=1:hcmN
        if hcmAdjacencyMatrix(i,j) == 1
            fprintf(fileID,'<edge id="%d" source="%d" target="%d"/>',edgesCount,i,j);
            edgesCount = edgesCount + 1;
        end
    end
end
fprintf(fileID,'</edges>');
fprintf(fileID,'</graph>');fprintf(fileID,'</gexf>');
fclose(fileID);