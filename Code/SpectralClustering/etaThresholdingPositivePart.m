function [ y ] = etaThresholdingPositivePart( x )
    n = length(x);
    y = x;
    for i=1:n
        if x(i) <= 0
            y(i) = 0;
        end
    end
end