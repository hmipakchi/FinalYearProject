function [ ddkr2Estimate ] = DDKRAlgorithm2( responseMatrix )
%   DDKRAlgorithm2
%   Input: response matrix
%   Output: estimate of tasks using Dalvi et al. (DDKR) Algorithm 2 Method

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

    userUserResponseMatrix = responseMatrix' * responseMatrix;
    userUserAssignmentMatrix = assignmentMatrix' * assignmentMatrix;
    
    [v1,lamda1] = eigs(HadamardProduct2(userUserResponseMatrix, userUserAssignmentMatrix),1);
    if v1(1) < 0
        v1 = -1 .* v1;
    end
    
    % modify the user-user assignment matrix by applying indicator function
    for i=1:n
        for j=1:n
            if userUserAssignmentMatrix(i,j) ~= 0
                userUserAssignmentMatrix(i,j) = 1;
            else
                userUserAssignmentMatrix(i,j) = 0;
            end
        end
    end
    [v2,lamda2] = eigs(userUserAssignmentMatrix,1);
    if v2(1) < 0
        v2 = -1 .* v2;
    end
    
    wHat = HadamardProduct2(v1, v2);
    
    wTilda = zeros(n,1);
    for j=1:n
        if wHat(j) >= 0
            wTilda(j) = max(abs(wHat(j)),1);
        else
            wTilda(j) = -1 * max(abs(wHat(j)),1);
        end
    end
    
    ddkr2Estimate = zeros(m,1);
    for i=1:m
        sum = 0;
        for j=1:n
            sum = sum + responseMatrix(i,j) * wTilda(j);
        end
        if sum >= 0
            ddkr2Estimate(i) = 1;
        else
            ddkr2Estimate(i) = -1;
        end
    end
    
end