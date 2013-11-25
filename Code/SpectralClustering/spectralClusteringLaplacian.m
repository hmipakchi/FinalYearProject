function [ communityAssignments ] = spectralClusteringLaplacian( adjacencyMatrix, k )
%testLaplacianSpectralClustering: Implements Spectral Clustering based on Laplcian matrix
%   Inputs: Adjacency matrix of a graph, number of communities (k)
%   Outputs: Indicators matrix classfying each node in the graph to a
%   particular community

    [noRows, noCols] = size(adjacencyMatrix);
    n = noRows;

    % degree matrix
    degreeMatrix = zeros(n,n);
    for i=1:n
        degreeMatrix(i,i) = sum(adjacencyMatrix(i,:));
    end

    % Laplacian matrix, L = D - A
    laplacianMatrix = degreeMatrix - adjacencyMatrix;

    % calculate eigenvectors of Laplacian matrix
    [eigVecM, eigValM] = eig(laplacianMatrix);

    % only consider first k eigenvectors and eigenvalues
    [rows, cols] = size(eigVecM);
    eigVec = zeros(rows, k);
    eigVal = zeros(k, 1);

    % first eigenvector has all elements = 1, so provides no information
    for i = 2:k
        eigVec(:, i-1) = eigVecM(:, i);
        eigVal(i-1, 1) = eigValM(i, i);
    end

    % create embedded vectors and plot graph
    embedVec = transpose(eigVec);

    [indicators, clusterCentres] = testKMeansClustering(transpose(embedVec(1:(k-1),:)), k);
    
    communityAssignments = zeros(n,1);
    
    for i=1:n
        for j=1:k
            if indicators(i,j) == 1
                communityAssignments(i,1) = j;
            end
        end
    end

end

