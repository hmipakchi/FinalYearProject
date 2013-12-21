function [ y ] = etaPrimeThresholding( x, theta )
    n = length(x);
    y = zeros(n,1);
    for i=1:n
        if x(i) > theta
            y(i) = 1;
        elseif x < -theta
            y(i) = 1;
        else
            y(i) = 0;
        end
    end
end