function [ ampEstimate ] = AMPAlgorithm( responseMatrix, noIterations)
%   AMPAlgorithm
%   Input: response matrix, number of iterations to run
%   Output: estimate of tasks using AMP Method

    [m,n] = size(responseMatrix);
    
    u = zeros(m,1);
    z = randn(n,1);
    
    for i = 1:noIterations
        gamma = u + (responseMatrix * z);
%         u_prev = u;
        u = etaThresholdingNonNegative(gamma);
        beta = (1/n) * sum(etaPrimeThresholdingNonNegative(gamma));
        z = (responseMatrix' * u) + (beta * z);
%         z = (responseMatrix' * u) - (beta * u_prev);
    end
    
    ampEstimate = zeros(m,1);
    epsilon = 0.1;
    for i=1:m
        if u(i) > epsilon
            ampEstimate(i) = 1;
        else
            ampEstimate(i) = -1;
        end
    end

end
