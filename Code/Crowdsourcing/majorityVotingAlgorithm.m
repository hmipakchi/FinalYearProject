function [ mvEstimate ] = majorityVotingAlgorithm( responseMatrix )
%   majorityVotingAlgorithm
%   Input: response matrix,
%   Output: estimate of tasks using Majority Voting Method

    [m,n] = size(responseMatrix);

    mvEstimate = zeros(m,1);
    for i=1:m
        if sum(responseMatrix(i,:)) >= 0
            mvEstimate(i) = 1;
        else
            mvEstimate(i) = -1;
        end
    end

end

