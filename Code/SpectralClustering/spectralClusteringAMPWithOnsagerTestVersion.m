function [ communityAssignments ] = spectralClusteringAMPWithOnsagerTestVersion( adjacencyMatrix, noIterations )
%spectralClusteringAMP: Implements Spectral Clustering based on Approximate
%                       Message Passing (AMP) algorithm
%   Inputs: Adjacency matrix of a graph - adjacencyMatrix
%           Number of iterations to run - noIterations
%   Outputs: Indicators vector -> communityAssignments

    [noRows, noCols] = size(adjacencyMatrix);
    n = noRows;
   
    u_prev = zeros(n,1);
    u = ones(n,1);
    z = zeros(n,1);
%     z = rand(n,1);
        
    for t=1:noIterations
%         theta = abs(mean(z));
%         beta = (1/n) * sum(etaPrimeThresholdingSoft(z,theta));
        beta = (1/n) * sum(positivePartPrimeThresholding(z));
        z = (adjacencyMatrix * u) - (beta * u_prev);
        u_prev = u;
        u = positivePartThresholding(z);
%         u = etaThresholdingSoft(z, theta);
    end
    
   communityAssignments = zeros(n,1);

    if (norm(u) ~= 0)
        communityAssignments = u/norm(u);
    end
    
end

function [ y ] = positivePartThresholding(x)
    n = length(x);
    y = x;
    for i=1:n
        if x(i) <= 0
            y(i) = 0;
        end
    end
end

function [ y ] = positivePartPrimeThresholding(x)
    n = length(x);
    y = ones(n,1);
    for i=1:n
        if x(i) <= 0
            y(i) = 0;
        end
    end
end
