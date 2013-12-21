clear;

% definition of the stochastic block model

% number vertices
n = 100;
n_str = int2str(n);

% number of communities
q = 2;
q_str = int2str(q);

pinValueVector = [0.3; 0.9];
noColumnsInPlot = 1;

for pinValueNo=1:max(size(pinValueVector))
    
    % probability matrix indicating the proability of edge between communities
    pout = 0.0;
    pin = pinValueVector(pinValueNo,1);
    pin_str = num2str(pin);
    cin = floor(pin*n);
    cout = floor(pout*n);
    % let number of iterations be proportional to number of vertices in
    % graph
    maxNoIterations = n;
    deltaCout = (cin - cout) / maxNoIterations;

    errorsMatrix = zeros(maxNoIterations, 6);
    modularityMethodLowerBound = 0.8*1/q;

    for iteration=1:maxNoIterations
        display(pinValueNo);
        display(iteration);
        
        affinityMatrix = (pout .* ones(q,q)) + ((pin-pout) .* eye(q,q));

        % choose communitiy for each edge (i.e. 1,...,q)
        nodeCommunities = zeros(n, 1);
        for i=1:n
            nodeCommunities(i,1) = mod((i-1), q) + 1; 
        end

        % create adjacency matrix for this stochastic block model
        adjacencyMatrix = zeros(n,n);
        for i=1:n
            for j=1:n
                if i ~= j
                    flipResult = rand(1);
                    if flipResult <= affinityMatrix(nodeCommunities(i,1),nodeCommunities(j,1))
                        adjacencyMatrix(i,j) = 1;
                    end
                end
            end
        end

        % apply spectral clustering using Laplacian method
        communityAssignmentsLaplacian = spectralClusteringLaplacian(adjacencyMatrix, q);
        
        % apply spectral clustering using Modularity methos
        communityAssignmentsModularity = spectralClusteringModularity(adjacencyMatrix, q);

        % check errors in community detection
        errorsCommunityDetection = zeros(n, 2);

        for i=1:n
            if communityAssignmentsLaplacian(i,1) == nodeCommunities(i,1)
                errorsCommunityDetection(i,1) = 0;
            else
                errorsCommunityDetection(i,1) = 1;
            end
            if communityAssignmentsModularity(i,1) == nodeCommunities(i,1)
                errorsCommunityDetection(i,2) = 0;
            else
                errorsCommunityDetection(i,2) = 1;
            end
        end
        percentageErrorsCommunityDetectionLaplacian = sum(errorsCommunityDetection(:,1)) / n * 100;
        percentageErrorsCommunityDetectionModularity = sum(errorsCommunityDetection(:,2)) / n * 100;
        if percentageErrorsCommunityDetectionModularity > 100 * (1 - modularityMethodLowerBound)
            percentageErrorsCommunityDetectionModularity = 100 - percentageErrorsCommunityDetectionModularity;
        end

        errorsMatrix(iteration, 1) = cin;
        errorsMatrix(iteration, 2) = cout;
        errorsMatrix(iteration, 3) = cout / cin;
        errorsMatrix(iteration, 4) = 1 - (percentageErrorsCommunityDetectionLaplacian / 100);
        errorsMatrix(iteration, 5) = 1 - (percentageErrorsCommunityDetectionModularity / 100);
        errorsMatrix(iteration, 6) = abs(cin - cout) - (q*sqrt((1/q*cin)+(q-1/q*cout)));

        pout = pout + deltaCout/n;
        cout = cout + deltaCout;
    end
    
    subplot(floor((max(size(pinValueVector))-1)/noColumnsInPlot)+1, noColumnsInPlot, pinValueNo);
    
    title_str = sprintf('plot for laplacian (blue) and modularity (red) methods [n = %s, q = %s, pin = %s]',n_str,q_str,pin_str);
    title(title_str,'FontSize',16);
    xlabel('cout / cin','FontSize',17);
    ylabel('success rate','FontSize',16);
    circlePlotSize = 100;
    
    for i=1:maxNoIterations
        hold on;
        
        %errorsMatrix(i,3) > ((errorsMatrix(i,1) + 1 - sqrt((4*errorsMatrix(i,1))+1)) / errorsMatrix(i,1))
        if errorsMatrix(i,6) <= 0
            scatter(errorsMatrix(i,3),errorsMatrix(i,4),circlePlotSize,'blue');
        else
            scatter(errorsMatrix(i,3),errorsMatrix(i,4),circlePlotSize,'blue');
        end
        if errorsMatrix(i,6) <= 0
            scatter(errorsMatrix(i,3),errorsMatrix(i,5),circlePlotSize,'red');
        else
            scatter(errorsMatrix(i,3),errorsMatrix(i,5),circlePlotSize,'red');
        end
    end
    
end