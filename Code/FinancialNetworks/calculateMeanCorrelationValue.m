function [ meanValue ] = calculateMeanCorrelationValue( sampleCrossCorrelationMatrix )
    [n,noCols] = size(sampleCrossCorrelationMatrix);
    meanValue = sum(sum(sampleCrossCorrelationMatrix)) / (n^2);
end