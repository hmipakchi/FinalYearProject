function [ ddkr1Estimate ] = DDKRAlgorithm1( responseMatrix )
%   DDKRAlgorithm1
%   Input: response matrix
%   Output: estimate of tasks using Dalvi et al. (DDKR) Algorithm 1 Method

    [m,n] = size(responseMatrix);
    
    % create assignment matrix from response matrix
    assignmentMatrix = zeros(m,n);
    for i=1:m
        for j=1:n
            if responseMatrix(i,j) ~= 0
                assignmentMatrix(i,j) = 1;
            else
                assignmentMatrix(i,j) = 0;
            end
        end
    end

    [vR,lamdaR] = eigs(responseMatrix' * responseMatrix,1);
    if vR(1) < 0
        vR = -1 .* vR;
    end
    
    [vA,lamdaA] = eigs(assignmentMatrix' * assignmentMatrix,1);
    if vA(1) < 0
        vA = -1 .* vA;
    end
    
    wHat = HadamardProduct2(vR, vA);
    
    wTilda = zeros(n,1);
    for j=1:n
        if wHat(j) >= 0
            wTilda(j) = max(abs(wHat(j)),1);
        else
            wTilda(j) = -1 * max(abs(wHat(j)),1);
        end
    end
    
    ddkr1Estimate = zeros(m,1);
    for i=1:m
        sum = 0;
        for j=1:n
            sum = sum + responseMatrix(i,j) * wTilda(j);
        end
        if sum >= 0
            ddkr1Estimate(i) = 1;
        else
            ddkr1Estimate(i) = -1;
        end
    end
    
end