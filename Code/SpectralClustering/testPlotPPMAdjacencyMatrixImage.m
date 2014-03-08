clear;

% definition of the stochastic block model

% number vertices
ppmN = 300;%300

% number of communities
ppmQ = 3;
    
% probability matrix indicating the proability of edge between communities
ppmPout = 0.3;%0.3
ppmPin = 0.7;%0.7

ppmCin = floor(ppmPin*ppmN);
ppmCout = floor(ppmPout*ppmN);

ppmAffinityMatrix = (ppmPout .* ones(ppmQ,ppmQ)) + ((ppmPin-ppmPout) .* eye(ppmQ,ppmQ));
whichModel_str = 'ppm';

% choose communitiy for each edge (i.e. 1,...,q)
ppmNodeCommunities = zeros(ppmN,1);
for i=1:ppmN
    nodeCommunitity = mod((i-1), ppmQ) + 1;
    ppmNodeCommunities(i) = nodeCommunitity;
end

% evaluate number of nodes per community
ppmNumberNodesPerCommunity = zeros(ppmQ,1);
for i=1:ppmN
    nodeCommunitity = ppmNodeCommunities(i);
    ppmNumberNodesPerCommunity(nodeCommunitity) = ppmNumberNodesPerCommunity(nodeCommunitity) + 1;
end

% evaluate cumulative number of nodes up to a community
ppmCumulativeNumberNodesUptoCommunity = zeros(ppmQ,1);
for i=1:ppmQ
    if i >= 2
        ppmCumulativeNumberNodesUptoCommunity(i) = ppmCumulativeNumberNodesUptoCommunity(i-1) + ppmNumberNodesPerCommunity(i-1);
    end
end

% evaluate all node members for each community
ppmCommunityNodeMemberships = -1.*ones(ppmQ,max(ppmNumberNodesPerCommunity));
ppmCommunityNodeMembershipsIterators = ones(ppmQ,1);
for i=1:ppmN
    nodeCommunitity = ppmNodeCommunities(i);
    ppmCommunityNodeMemberships(nodeCommunitity,ppmCommunityNodeMembershipsIterators(nodeCommunitity)) = i;
    ppmCommunityNodeMembershipsIterators(nodeCommunitity) = ppmCommunityNodeMembershipsIterators(nodeCommunitity) + 1;
end

% create adjacency matrix for this stochastic block model
ppmAdjacencyMatrix = zeros(ppmN,ppmN);
for i=1:ppmN
    for j=1:ppmN
        % no self edges allowed
        if i ~= j
            flipResult = rand(1);
            if flipResult <= ppmAffinityMatrix(ppmNodeCommunities(i),ppmNodeCommunities(j))
                ppmAdjacencyMatrix(i,j) = 1;
            end
        end
    end
end

% labelled adjacency matrix for this stochastic block model to demonstrate
% communities
ppmLabelledAdjacencyMatrix = zeros(ppmN,ppmN);
for c1=1:ppmQ
    for c2=1:ppmQ
        for i=1:ppmNumberNodesPerCommunity(c1)
            for j=1:ppmNumberNodesPerCommunity(c2)
                rowValueAdjMatrix = ppmCommunityNodeMemberships(c1,i);
                colValueAdjMatrix = ppmCommunityNodeMemberships(c2,j);
                rowValueLabelledAdjMatrix = ppmCumulativeNumberNodesUptoCommunity(c2) + j;
                colValueLabelledAdjMatrix = ppmCumulativeNumberNodesUptoCommunity(c1) + i;
                ppmLabelledAdjacencyMatrix(rowValueLabelledAdjMatrix,colValueLabelledAdjMatrix) = ppmAdjacencyMatrix(rowValueAdjMatrix,colValueAdjMatrix);
            end
        end
    end
end

% write Adjacency Matrix to file
adjacencyMatrixFilename = sprintf('data_files/adjacencyMatrices/%sAdjacencyMatrix.dat',whichModel_str);
fileID = fopen(adjacencyMatrixFilename,'w');
for i=ppmN:-1:1
    for j=1:ppmN
        if j ~= ppmN
            fprintf(fileID,'%d ',ppmAdjacencyMatrix(i,j));
        else
            fprintf(fileID,'%d',ppmAdjacencyMatrix(i,j));
        end
    end
    fprintf(fileID,'\n');
end
fclose(fileID);

% write Labelled Adjacency Matrix to file
labelledAdjacencyMatrixFilename = sprintf('data_files/adjacencyMatrices/%sLabelledAdjacencyMatrix.dat',whichModel_str);
fileID = fopen(labelledAdjacencyMatrixFilename,'w');
for i=ppmN:-1:1
    for j=1:ppmN
        if j ~= ppmN
            fprintf(fileID,'%d ',ppmLabelledAdjacencyMatrix(i,j));
        else
            fprintf(fileID,'%d',ppmLabelledAdjacencyMatrix(i,j));
        end
    end
    fprintf(fileID,'\n');
end
fclose(fileID);

% write Adjacency Matrix to gexf file
% adjacencyMatrixGEXFFilename = sprintf('data_files/adjacencyMatrices/%sExampleGraph.gexf',whichModel_str);
% fileID = fopen(adjacencyMatrixGEXFFilename,'w');
% fprintf(fileID,'<gexf version="1.1"><meta><creator>Hesam Ipakchi</creator></meta><graph defaultedgetype="directed" idtype="string" type="static">');
% fprintf(fileID,'<nodes count="%d">',ppmN);
% for i=1:ppmN
%     fprintf(fileID,'<node id="%d" label="%d"/>',i,i);
% end
% fprintf(fileID,'</nodes>');
% ppmNodeDegrees = sum(ppmAdjacencyMatrix);
% ppmTotalNoEdges = sum(ppmNodeDegrees);
% fprintf(fileID,'<edges count="%d">',ppmTotalNoEdges);
% edgesCount = 1;
% for i=1:ppmN
%     for j=1:ppmN
%         if ppmAdjacencyMatrix(i,j) == 1
%             fprintf(fileID,'<edge id="%d" source="%d" target="%d"/>',edgesCount,i,j);
%             edgesCount = edgesCount + 1;
%         end
%     end
% end
% fprintf(fileID,'</edges>');
% fprintf(fileID,'</graph>');fprintf(fileID,'</gexf>');
% fclose(fileID);