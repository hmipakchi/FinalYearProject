clear;

% k colours
k = 3;

% number vertices
n = 150;

% probability of edge within communities, p, and between communities, q
p = 0.5;
q = 0.1;

% choose communitiy for each edge (i.e. 0,...,k-1)
nodeCommunities = zeros(n, 1);
for i=1:n
    nodeCommunities(i,1) = mod((i-1), k); 
end

adjacencyMatrix = zeros(n,n);
for i=1:n
    for j=1:n
        if i ~= j
            if nodeCommunities(i,1) == nodeCommunities(j,1)
                flipResult = rand(1);
                if flipResult <= p
                    adjacencyMatrix(i,j) = 1;
                end
            else
                flipResult = rand(1);
                if flipResult <= q
                    adjacencyMatrix(i,j) = 1;
                end
            end
        end
    end
end

% degree matrix is a diagonal matrix with ith element in diagonal equal to
% degree of vertex i
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

title('embedded vectors plot','FontSize',18);
xlabel('u1','FontSize',18);
ylabel('u2','FontSize',18);
circlePlotSize = 100;
for v=1:rows
    hold on;
    if nodeCommunities(v,1) == 0
        scatter(embedVec(1,v),embedVec(2, v),circlePlotSize,'r')
    elseif nodeCommunities(v,1) == 1
        scatter(embedVec(1,v),embedVec(2, v),circlePlotSize,'g')
    else
        scatter(embedVec(1,v),embedVec(2, v),circlePlotSize,'b')
    end
end

[indicators, clusterCentres] = testKMeansClustering(transpose(embedVec(1:(k-1),:)), k);

% check errors in community detection
errorsCommunityDetection = zeros(n, 1);
for i=1:n
    if indicators(i,nodeCommunities(i,1)+1) == 1
        errorsCommunityDetection(i,1) = 0;
    else
        errorsCommunityDetection(i,1) = 1;
    end
end
percentageErrorsCommunityDetection = sum(errorsCommunityDetection(:,1)) / n * 100;
display(percentageErrorsCommunityDetection);
