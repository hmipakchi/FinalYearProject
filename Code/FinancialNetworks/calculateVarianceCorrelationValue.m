function [ variance ] = calculateVarianceCorrelationValue( sampleCrossCorrelationMatrix )
    meanValue = calculateMeanCorrelationValue(sampleCrossCorrelationMatrix);
    variance = 0;
    [n,noCols] = size(sampleCrossCorrelationMatrix);
    for i=1:n
        for j=1:n
            variance = variance + ((sampleCrossCorrelationMatrix(i,j) - meanValue)^2);
        end
    end
    variance = variance / (n^2);
end