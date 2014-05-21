function [ communityAssignments ] = financialSpectralClustering( adjacencyMatrix, k, SNR )
    n = size(adjacencyMatrix, 1);
    deg = full(sum(adjacencyMatrix)');
    m = sum(deg)/2;
%     modularityMatrix = full(adjacencyMatrix) - deg*deg'/(2*m);
    modularityMatrix = adjacencyMatrix;
    
    % calculate eigenvectors of Modularity matrix
    [eigVecM, eigValM] = eig(modularityMatrix);
    
    communityAssignments = zeros(n,1);
    
    % only consider first k eigenvectors and eigenvalues
    eigVec = zeros(n,k);
    eigVal = zeros(k,1);

    for i = 1:k
%         eigVec(:,k-i+1) = eigVecM(:,n-i+1);
%         eigVal(k-i+1,k-i+1) = eigValM(n-i+1,n-i+1);
        eigVec(:,i) = eigVecM(:,n-i+1);
        eigVal(i,i) = eigValM(n-i+1,n-i+1);
    end

    % create embedded vectors and plot graph
    embedVec = eigVec';
    
    filename = sprintf('../data_files/financialNetworks/syntheticCorrelationMatrices_ModularityMatrix_Eigenvectors_top_%s.dat',num2str(k));
%     filename = sprintf('../data_files/financialNetworks/financialSpectralClustering_ModularityMatrix_Eigenvectors_top_%s.dat',num2str(k));
    fileID = fopen(filename,'w');
    for i=1:n
        for j=1:k
            if (j ~= k)
                fprintf(fileID,'%d ',eigVec(i,j));
            else
                fprintf(fileID,'%d',eigVec(i,j));
            end
        end
        if (i ~= n)
            fprintf(fileID,'\n');
        end
    end
    fclose(fileID);

    [communityAssignments,clusterCentres] = kmeans(embedVec',k,'distance','cityblock');
    
%     [indicators, clusterCentres] = testKMeansClustering(embedVec', k);

%     [noDataPoints, noClusters] = size(indicators);
%     for i=1:noDataPoints
%         for j=1:noClusters
%             if (indicators(i,j) == 1)
%                 communityAssignments(i) = j;
%             end
%         end
%     end

end

