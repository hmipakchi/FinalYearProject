function [ meanProductTimeAverage ] = calculateMeanProductTimeAverage( timeSeries )
%   calculateMeanProductTimeAverage
%   Input: time series
%   Output: mean product time average of time series

    [n,T] = size(timeSeries);
    
    meanProductTimeAverage = zeros(n,n);
    
    for i=1:n
        for j=1:n
            meanProductTimeAverageSum = 0;
            for t=1:T
                meanProductTimeAverageSum = meanProductTimeAverageSum + (timeSeries(i,t) * timeSeries(j,t));
            end
            meanProductTimeAverage(i,j) = meanProductTimeAverageSum / T;
        end
    end

end
