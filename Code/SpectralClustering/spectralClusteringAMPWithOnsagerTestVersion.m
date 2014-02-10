function [ communityAssignments ] = spectralClusteringAMPWithOnsagerTestVersion( adjacencyMatrix, noIterations )
%spectralClusteringAMP: Implements Spectral Clustering based on Approximate
%                       Message Passing (AMP) algorithm
%   Inputs: Adjacency matrix of a graph - adjacencyMatrix
%           Number of iterations to run - noIterations
%   Outputs: Indicators vector -> communityAssignments

    [noRows, noCols] = size(adjacencyMatrix);
    n = noRows;
    
    u = zeros(n,1);
    z = rand(n,1);
    
    for t=1:noIterations
        % keep theta non-negative
        theta = abs(mean(z));
        u_prev = u;
%         u = etaThresholdingSoft(z, theta);
%         beta = (1/n) * sum(etaPrimeThresholdingSoft(z,theta));
        u = positivePartThresholding(z);
        beta = (1/n) * sum(positivePartPrimeThresholding(z));
        z = (adjacencyMatrix * u) - (beta * u_prev);
    end
    
   communityAssignments = zeros(n,1);
%     epsilon = 0.1;
%     for i=1:n
%         if abs(u(i)) > epsilon
%             communityAssignments(i) = 1;
%         else
%             communityAssignments(i) = 0;
%         end
%     end

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
