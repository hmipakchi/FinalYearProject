function [ communityAssignments ] = financialSpectralClustering( modularityMatrix, maxK )


    [noRows, noCols] = size(modularityMatrix);
    n = noRows;
    
    % calculate eigenvectors of Modularity matrix
    [eigVecM, eigValM] = eig(modularityMatrix);
    
    communityAssignments = zeros(n,maxK);
    
    for k=1:maxK

        % only consider first k eigenvectors and eigenvalues
        eigVec = zeros(n,k);
        eigVal = zeros(k,1);

        % first eigenvector has all elements = 1, so provides no information
        for i = 1:k
            eigVec(:,i) = eigVecM(:,i);
            eigVal(i,1) = eigValM(i,i);
        end

        % create embedded vectors and plot graph
        embedVec = eigVec';
        
        [indicators, clusterCentres] = testKMeansClustering(embedVec', k);
    
        [noDataPoints, noClusters] = size(indicators);
        for i=1:noDataPoints
            for j=1:noClusters
                if (indicators(i,j) == 1)
                    communityAssignments(i,k) = j;
                end
            end
        end
    end

end

