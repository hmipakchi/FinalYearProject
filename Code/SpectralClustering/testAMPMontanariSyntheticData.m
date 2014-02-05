clear;

% definition of the stochastic block model

% number vertices
n = 200;%500
n_str = int2str(n);

% number of communities
q = 2;
q_str = int2str(q);

% number of iterations of algorithm
t = 50;

noLambdaPoints = 50;%100
lambdaMin = 0;
lambdaMax = 1.2;
deltaLambda = (lambdaMax - lambdaMin) / noLambdaPoints;
lambdaIterationNo = 1;

lambdaVector = zeros(noLambdaPoints,1);
poutVector = zeros(noLambdaPoints,1);

pin = 0.25;
pin_str = num2str(pin);

scalarProductsBayesAMP = zeros(noLambdaPoints, noLambdaPoints);
scalarProductsBayesAMPWithOnsager = zeros(noLambdaPoints, noLambdaPoints);

for lambda=lambdaMin+deltaLambda:deltaLambda:(lambdaMax)
    disp(lambdaIterationNo);
    
    % proability of edge between communities
    [ pout1, pout2 ] = solveQuadraticEquation( 1+(lambda*n), -1*((2*pin)+(lambda*n)), pin*pin );
    pout = pout2;
    
    lambdaVector(lambdaIterationNo) = lambda;
    poutVector(lambdaIterationNo) = pout;
    
     % create affinity matrix for this stochastic block model
    adjustedBackgroundPertubationMatrix = zeros(q,q);
    adjustedBackgroundPertubationMatrix(1,1) = 1;
    affinityMatrix = (pout .* ones(q,q)) + ((pin-pout) .* adjustedBackgroundPertubationMatrix);
    
    kIterationNo = 1;
    for k=1:(n/noLambdaPoints):n
        % choose communitiy for each edge (i.e. 0,...,q-1)
        nodeCommunities = zeros(n, 1);
        for i=1:n
            if (i <= k)
                nodeCommunities(i,1) = 1;
            else
                nodeCommunities(i,1) = 0;
            end
        end

        % create adjacency matrix for this stochastic block model
        adjacencyMatrix = (lambda.*(nodeCommunities * nodeCommunities')) + randn(n,n);
        
        if (lambdaIterationNo == 10) && (kIterationNo == 50)
            debugPoint = 1;
        end

        % apply spectral clustering using Approximate Message Passing (AMP) method
        % Note: only works for 2 communities -> q = 2 !!!
        communityAssignmentsAMP = spectralClusteringAMP(adjacencyMatrix, pout, t);

        % apply spectral clustering using Approximate Message Passing (AMP) method
        % Note: only works for 2 communities -> q = 2 !!!
        communityAssignmentsAMPWithOnsager = spectralClusteringAMPWithOnsager(adjacencyMatrix, pout, t);
        
        % calculate scalar products of estimate
        if (norm(communityAssignmentsAMP,2)) ~= 0
            scalarProductsBayesAMP(lambdaIterationNo,kIterationNo) = nodeCommunities' * communityAssignmentsAMP / (norm(nodeCommunities,2)*norm(communityAssignmentsAMP,2));
        else
            scalarProductsBayesAMP(lambdaIterationNo,kIterationNo) = 0;
        end
        if (norm(communityAssignmentsAMPWithOnsager,2)) ~= 0
            scalarProductsBayesAMPWithOnsager(lambdaIterationNo,kIterationNo) = nodeCommunities' * communityAssignmentsAMPWithOnsager / (norm(nodeCommunities,2)*norm(communityAssignmentsAMPWithOnsager,2));
        else
            scalarProductsBayesAMPWithOnsager(lambdaIterationNo,kIterationNo) = 0;
        end
        kIterationNo = kIterationNo + 1;
    end
    lambdaIterationNo = lambdaIterationNo + 1;
end

% write algorithms scalar products to file
    filename_str = sprintf('data_files/spectralClustering/AMPMontanariSyntheticDataScalarProducts_pin_%s.dat',pin_str);
    fileID = fopen(filename_str,'w');
    for lambdaIterationNo=1:length(lambdaVector)
        kIterationNo = 1;
        for k=1:(n/noLambdaPoints):n
            epsilon = k / n;
            fprintf(fileID,'%d ',epsilon);
            fprintf(fileID,'%d ',lambdaVector(lambdaIterationNo));
            fprintf(fileID,'%d ',scalarProductsBayesAMP(lambdaIterationNo,kIterationNo));
            fprintf(fileID,'%d ',scalarProductsBayesAMPWithOnsager(lambdaIterationNo,kIterationNo));
            fprintf(fileID,'\n');
            kIterationNo = kIterationNo + 1;
        end
    end
    fclose(fileID);