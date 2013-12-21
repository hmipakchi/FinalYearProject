function [ svEstimate ] = singularVectorAlgorithm( responseMatrix, noIterations )
%   singularVectorAlgorithm
%   Input: response matrix, number of iterations to run
%   Output: estimate of tasks using Singular Vector Method

    [m,n] = size(responseMatrix);
    
    u = zeros(m,1);
    v = -1 + 2.*rand(n,1);
    
    for i=1:noIterations
        u = responseMatrix * v;
        v = responseMatrix' * u;
    end
    
    svEstimate = zeros(m,1);
    for i=1:m
        if u(i) >= 0
            svEstimate(i) = 1;
        else
            svEstimate(i) = -1;
        end
    end

end

