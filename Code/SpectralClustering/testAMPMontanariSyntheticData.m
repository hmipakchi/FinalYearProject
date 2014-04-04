clear;

% definition of the stochastic block model

% number vertices
n = 250;%500
n_str = int2str(n);

% number of iterations of algorithm
t = 25;%50

noLambdaPoints = 50;%100
lambdaMin = 0;
lambdaMax = 1.2;
deltaLambda = (lambdaMax - lambdaMin) / noLambdaPoints;
lambdaIterationNo = 1;

lambdaVector = zeros(noLambdaPoints,1);
% qVector = zeros(noLambdaPoints,1);
% pVector = zeros(noLambdaPoints,1);

scalarProductsAMP = zeros(noLambdaPoints,noLambdaPoints);
scalarProductsAMPWithOnsager = zeros(noLambdaPoints,noLambdaPoints);

noMCIterations = 1;%1

for MCIt=1:noMCIterations
    lambdaIterationNo = 1;

for lambda=lambdaMin+deltaLambda:deltaLambda:(lambdaMax)
    disp(MCIt);
    disp(lambdaIterationNo);
    
    lambdaVector(lambdaIterationNo) = lambda;
    
%     p = 1.0;
%     [q1,q2] = solveQuadraticEquation(1+(lambda*lambda*n),-(2*p)-(lambda*lambda*n),p*p);
%     q = q2;
%     qVector(lambdaIterationNo) = q;
    
    kIterationNo = 1;
    for k=1:ceil(n/noLambdaPoints):n
        
        % choose communitiy for each node
        nodeCommunities = zeros(n,1);
        nodeCommunities(1:k) = 1;
        epsilon = k/n;
        
%         nodeCommunities = binornd(1,epsilon,n,1);

        % create adjacency matrix for this stochastic block model
%         adjacencyMatrix = zeros(n,n);
%         for i=1:n
%             for j=i:n
%                 if i ~= j
%                     flipResult = rand(1);
%                     if (nodeCommunities(i) == 1) && (nodeCommunities(j) == 1)
%                         if flipResult <= p
%                             adjacencyMatrix(i,j) = 1;
%                         else
%                             adjacencyMatrix(i,j) = 0;
%                         end
%                     else
%                         if flipResult <= q
%                             adjacencyMatrix(i,j) = 1;
%                         else
%                             adjacencyMatrix(i,j) = 0;
%                         end
%                     end
%                 else
%                     adjacencyMatrix(i,j) = 0;
%                 end
%                 adjacencyMatrix(j,i) = adjacencyMatrix(i,j);
%             end
%         end
%         adjacencyMatrix = (lambda/(p-q)).*(adjacencyMatrix - (q.*ones(n,n)));

        Z = sqrt(1/n).*randn(n,n);
        adjacencyMatrix = (lambda/k).*(nodeCommunities * nodeCommunities') + Z;

        if (lambda > 0.8) && (k/n > 0.2)
            debug = 1;
        end

        % apply spectral clustering using Approximate Message Passing (AMP) method
        % Note: only works for 2 communities -> q = 2 !!!
        communityAssignmentsAMP = spectralClusteringAMPTestVersion(adjacencyMatrix, t);
        
        % apply spectral clustering using Approximate Message Passing (AMP) method
        % Note: only works for 2 communities -> q = 2 !!!
        communityAssignmentsAMPWithOnsager = spectralClusteringAMPWithOnsagerTestVersion(adjacencyMatrix, t);
        
        % calculate scalar products of estimate
        if (norm(nodeCommunities,2) ~= 0)
            scalarProductsAMP(lambdaIterationNo,kIterationNo) = scalarProductsAMP(lambdaIterationNo,kIterationNo) + (nodeCommunities' * communityAssignmentsAMP) / (norm(nodeCommunities,2));
            scalarProductsAMPWithOnsager(lambdaIterationNo,kIterationNo) = scalarProductsAMPWithOnsager(lambdaIterationNo,kIterationNo) + (nodeCommunities' * communityAssignmentsAMPWithOnsager) / (norm(nodeCommunities,2));
%         else
%             scalarProductsAMP(lambdaIterationNo,kIterationNo) = 0;
%             scalarProductsAMPWithOnsager(lambdaIterationNo,kIterationNo) = 0;
        end

        kIterationNo = kIterationNo + 1;
    end
    
    lambdaIterationNo = lambdaIterationNo + 1;
end

end

scalarProductsAMP = scalarProductsAMP / noMCIterations;
scalarProductsAMPWithOnsager = scalarProductsAMPWithOnsager / noMCIterations;

% write algorithms scalar products to file
filename_str = sprintf('data_files/spectralClustering/AMPMontanariSyntheticDataScalarProducts.dat');
fileID = fopen(filename_str,'w');
for lambdaIterationNo=1:length(lambdaVector)
    kIterationNo = 1;
    for k=1:ceil(n/noLambdaPoints):n
        epsilon = k/n;
        fprintf(fileID,'%d ',epsilon);
        fprintf(fileID,'%d ',lambdaVector(lambdaIterationNo));
        fprintf(fileID,'%d ',scalarProductsAMP(lambdaIterationNo,kIterationNo));
        fprintf(fileID,'%d ',scalarProductsAMPWithOnsager(lambdaIterationNo,kIterationNo));
        fprintf(fileID,'\n');
        kIterationNo = kIterationNo + 1;
    end
end
fclose(fileID);