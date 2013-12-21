function [ ampEstimate ] = AMPAlgorithm( responseMatrix, noIterations )
%   AMPAlgorithm
%   Input: response matrix, number of iterations to run
%   Output: estimate of tasks using Singular Vector Method

    [m,n] = size(responseMatrix);
    
    u = zeros(m,1);
    z = rand(n,1);
    
    for i = 1:noIterations
%         theta = abs(z);
        gamma = u + (responseMatrix * z);
        
        thresholdList = sort(abs(z), 'descend');
        theta = thresholdList(min(m,n));
        
        u = etaThresholding(gamma, theta);
        
        beta = (1/n) * sum(etaPrimeThresholding(gamma,theta));
        z = (responseMatrix' * u) - (beta * z);
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
