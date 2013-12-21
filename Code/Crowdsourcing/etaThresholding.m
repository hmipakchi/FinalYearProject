function [ y ] = etaThresholding( x, theta )
    n = length(x);
    y = zeros(n,1);
    for i=1:n
        if x(i) > theta
            y(i) = x(i) - theta;
        elseif x < -theta
            y(i) = x(i) + theta;
        else
            y(i) = 0;
        end
    end
end