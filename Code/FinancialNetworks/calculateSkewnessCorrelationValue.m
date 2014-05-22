function [ skewness ] = calculateSkewnessCorrelationValue( sampleCrossCorrelationMatrix )
    meanValue = calculateMeanCorrelationValue(sampleCrossCorrelationMatrix);
    standardDeviation = sqrt(calculateMeanCorrelationValue(sampleCrossCorrelationMatrix));
    skewness = 0;
    [n,noCols] = size(sampleCrossCorrelationMatrix);
    for i=1:n
        for j=1:noCols
            skewness = skewness + ((sampleCrossCorrelationMatrix(i,j) - meanValue)^3);
        end
    end
    skewness = skewness / (n^2);
    skewness = skewness / (standardDeviation^3);
    skewness = sqrt(n*(n-1))/(n-2) * skewness;
end