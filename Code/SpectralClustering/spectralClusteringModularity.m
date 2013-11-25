% Note: problem allocating detected clusters to cluster id's specified when
% initialising cluster id's to vertices.

function [ communityAssignments ] = spectralClusteringModularity( adjacencyMatrix, q )
%testLaplacianSpectralClustering: Implements Spectral Clustering based on Modularity matrix
%   Inputs: Adjacency matrix of a graph, number of communities (k)
%   Outputs: Indicators matrix classfying each node in the graph to a
%   particular community

    [noRows, noCols] = size(adjacencyMatrix);
    n = noRows;
    
    % degree matrix and total number of edges in the network
    degreeMatrix = zeros(n,n);
    noEdgesInNetwork = 0;
    for i=1:n
        degreeMatrix(i,i) = sum(adjacencyMatrix(i,:));
        noEdgesInNetwork = noEdgesInNetwork + degreeMatrix(i,i);
    end
    noEdgesInNetwork = noEdgesInNetwork / 2;
    
    % define modularity matrix
    modularityMatrix = zeros(n,n);
    for i=1:n
        for j=1:n
            modularityMatrix(i,j) = adjacencyMatrix(i,j) - (degreeMatrix(i,i) * degreeMatrix(j,j) / (2 * noEdgesInNetwork));
        end
    end
    
    % calculate the eigenvector associated with the largest eigenvalue
    [eigVecM, eigValM] = eig(modularityMatrix);
    leadingEigenVector = eigVecM(:,1);
    leadingEigenValue = eigValM(1,1);
    
    communityAssignments = zeros(n, 1);
    for i=1:n
        if leadingEigenValue > 0
            if leadingEigenVector(i,1) > 0
                communityAssignments(i,1) = 1;
            else
                communityAssignments(i,1) = 2;
            end
        else
            communityAssignments(i,1) = 1;
        end
    end

end

