function [ logReturns ] = calculateLogarithmicReturns( priceData )
%   calculateLogarithmicReturns
%   Input: price data
%   Output: logarithmic returns for price data

    [n,T] = size(priceData);
    logReturns = zeros(n,T);

    for i=1:n
        for t=1:T
            logReturn = 0;
            if t ~= 1
                logReturn = log(priceData(i,t)) - log(priceData(i,t-1));
            end
            logReturns(i,t) = logReturn;
        end
    end

end