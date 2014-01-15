function [ McrossN ] = HadamardProduct1( M, N )
%   HadamardProduct1
%   Input: M, N
%   Output: (McrossN)ij = Mij * Nij
    [m,n] = size(M);
    McrossN = zeros(m,n);
    for i=1:m
        for j=1:n
            McrossN(i,j) = M(i,j) * N(i,j);
        end
    end
end