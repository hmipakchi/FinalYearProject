function [ sampleVariance ] = calculateSampleVariance( timeSeries )
%   calculateSampleVariance
%   Input: time series
%   Output: sample variance of time series

    [n,T] = size(timeSeries);
    
    sampleVariance = zeros(n,1);
    
    timeAverage = calculateTimeAverage(timeSeries);
    meanSquareTimeAverage = calculateMeanSquareTimeAverage(timeSeries);
    
    for i=1:n
        sampleVariance(i) = meanSquareTimeAverage(i) - (timeAverage(i) * timeAverage(i));
    end

end
