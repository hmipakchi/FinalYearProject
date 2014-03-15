function [ communityAssignments ] = financialSpectralClusteringAMP( adjacencyMatrix, noIterations, noCommunities )
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
    
    preCommunityAssignments = zeros(n,noCommunities);
    
    for i=1:noCommunities
        for t=1:noIterations
            beta = (1/n) * sum(positivePartPrimeThresholding(z));
            z = (adjacencyMatrix * u) - (beta * u_prev);
            u_prev = u;
            u = positivePartThresholding(z);
            if (norm(u) ~= 0)
                u = u./norm(u);
            end
        end
        preCommunityAssignments(:,i) = u;
        % remove eigenvector component
        lambda = (u'*adjacencyMatrix*u) / (u'*u);
        adjacencyMatrix = adjacencyMatrix - (lambda.*(u*u'));
        % re-initialise z and u and u_prev
        u_prev = zeros(n,1);
        u = ones(n,1);
        z = zeros(n,1);
    end
    
    communityAssignments = zeros(n,1);
    for i=1:noCommunities
        threshold = mean(preCommunityAssignments(:,i));
        for j=1:n
            if preCommunityAssignments(j,i) > threshold
                communityAssignments(j) = i;
            end
        end
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
