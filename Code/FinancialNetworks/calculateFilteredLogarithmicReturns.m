function [ logReturns ] = calculateFilteredLogarithmicReturns( logReturns )
%   calculateFilteredLogarithmicReturns
%   Input: logarithmic returns
%   Output: filtered logarithmic returns

    [n,T] = size(logReturns);

    timeAverage = calculateTimeAverage(logReturns);
    sampleVariance = calculateSampleVariance(logReturns);

    for i=1:n
        for t=1:T
            logReturns(i,t) = (logReturns(i,t) - timeAverage(i)) / sqrt(sampleVariance(i));
        end
    end
end