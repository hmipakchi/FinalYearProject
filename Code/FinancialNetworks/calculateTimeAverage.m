function [ timeAverage ] = calculateTimeAverage( timeSeries )
%   calculateTimeAverage
%   Input: time series
%   Output: time average of time series

    [n,T] = size(timeSeries);
    
    timeAverage = zeros(n,1);
    
    for i=1:n
        timeAverageSum = 0;
        for t=1:T
            timeAverageSum = timeAverageSum + timeSeries(i,t);
        end
        timeAverage(i) = timeAverageSum / T;
    end

end
