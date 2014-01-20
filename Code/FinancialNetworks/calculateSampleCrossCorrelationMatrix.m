function [ sampleCrossCorrelationMatrix ] = calculateSampleCrossCorrelationMatrix( timeSeries )
%   calculateSampleCrossCorrelationMatrix
%   Input: time series
%   Output: sample cross correlation of time series

    [n,T] = size(timeSeries);
    
    sampleCrossCorrelationMatrix = zeros(n,n);
    
    timeAverage = calculateTimeAverage(timeSeries);
    sampleVariance = calculateSampleVariance(timeSeries);
    meanProductTimeAverage = calculateMeanProductTimeAverage(timeSeries);
    
    for i=1:n
        for j=1:n
            sampleCrossCorrelationMatrix(i,j) = (meanProductTimeAverage(i,j) - (timeAverage(i) * timeAverage(j))) / sqrt(sampleVariance(i) * sampleVariance(j));
        end
    end

end