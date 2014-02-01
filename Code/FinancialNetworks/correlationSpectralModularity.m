function [ communityAssignments ] = correlationSpectralModularity( correlationMatrix, T, k )
%correlationSpectralModularity: Implements Spectral Method based on
%Modularity of correlation matrix
%   Inputs: correlation matrix, number of time steps (T), number of communities (k)
%   Outputs: Indicators vector classfying each node to a community

    [noRows, noCols] = size(correlationMatrix);
    n = noRows;
    
    % construct null model for correlation matrix
    nullModelCorrelationMatrix = zeros(n,n);
    [eigVecC, eigValC] = eig(correlationMatrix);
    lambdaPlus = (1 + sqrt(n/T))^2;
    maxEigenvalue = max(max(eigValC));
    for i=1:n
        if ((eigValC(i,i) > lambdaPlus) && (eigValC(i,i) < maxEigenvalue))
            display(i);
            display(eigValC(i,i));
            nullModelCorrelationMatrix = nullModelCorrelationMatrix + (eigValC(i,i) .* (eigVecC(:,i) * eigVecC(:,i)'));
        end
    end
    
    % define modularity matrix
    modularityMatrix = zeros(n,n);
    for i=1:n
        for j=1:n
            modularityMatrix(i,j) = correlationMatrix(i,j) - nullModelCorrelationMatrix(i,j);
        end
    end
    
    % calculate the eigenvector associated with the largest eigenvalue
    [eigVecM, eigValM] = eig(modularityMatrix);

    % only consider first k eigenvectors and eigenvalues
    [rows, cols] = size(eigVecM);
    eigVec = zeros(rows, k);
    eigVal = zeros(k, 1);
    
    for i = 1:k
        eigVec(:,i) = eigVecM(:,i);
        eigVal(i,1) = eigValM(i,i);
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

