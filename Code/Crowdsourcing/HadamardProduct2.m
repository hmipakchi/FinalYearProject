function [ McrossN ] = HadamardProduct2( M, N )
%   HadamardProduct1
%   Input: M, N
%   Output: (McrossN)ij = if Nij != 0, Mij / Nij, and 0 otherwise
    [m,n] = size(M);
    McrossN = zeros(m,n);
    for i=1:m
        for j=1:n
            if N(i,j) ~= 0
                McrossN(i,j) = M(i,j) / N(i,j);
            else
                McrossN(i,j) = 0;
            end
        end
    end
end