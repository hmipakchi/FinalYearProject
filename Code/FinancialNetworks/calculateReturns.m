function [ returns ] = calculateReturns( priceData )
%   calculateReturns
%   Input: price data
%   Output: returns for price data

    [n,T] = size(priceData);
    returns = zeros(n,T);

    for i=1:n
        for t=1:T
            returnValue = 0;
            if t ~= 1
                returnValue = (priceData(i,t) - priceData(i,t-1)) / priceData(i,t-1);
            end
            returns(i,t) = returnValue;
        end
    end

end