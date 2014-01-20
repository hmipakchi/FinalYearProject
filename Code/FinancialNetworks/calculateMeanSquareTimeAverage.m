function [ meanSquareTimeAverage ] = calculateMeanSquareTimeAverage( timeSeries )
%   calculateMeanSquareTimeAverage
%   Input: time series
%   Output: mean square of time average of time series

    [n,T] = size(timeSeries);
    
    meanSquareTimeAverage = zeros(n,1);
    
    for i=1:n
        meanSquareTimeAverageSum = 0;
        for t=1:T
            meanSquareTimeAverageSum = meanSquareTimeAverageSum + (timeSeries(i,t) * timeSeries(i,t));
        end
        meanSquareTimeAverage(i) = meanSquareTimeAverageSum / T;
    end

end
