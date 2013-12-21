function [ oracleEstimate ] = oracleAlgorithm( responseMatrix, p )
%   oracleAlgorithm
%   Input: response matrix, reliability of each worker
%   Output: estimate of tasks using Oracle Estimator

    [m,n] = size(responseMatrix);
    
    n = length(p);
    
    oracleEstimate = zeros(m,1);
    
    for i=1:m
        hammerFound = 0;
        for j=1:n
            if (responseMatrix(i,j) ~= 0) && (hammerFound == 0)
                if p(j) == 1
                    oracleEstimate(i) = responseMatrix(i,j);
                    hammerFound = 1;
                end
            end
        end
        if hammerFound == 0
            flipResult = rand(1);
            if flipResult <= 0.5
                oracleEstimate(i) = -1;
            else
                oracleEstimate(i) = 1;
            end
        end
    end

end

