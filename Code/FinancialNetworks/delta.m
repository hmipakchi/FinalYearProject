function [ result ] = delta( i, j )
%delta
%   Kronecker Delta function

    result = 0;
    if (i == j)
        result = 1;
    end
end

