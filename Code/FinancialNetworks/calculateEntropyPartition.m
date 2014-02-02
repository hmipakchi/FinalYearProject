function [ entropy ] = calculateEntropyPartition( partition )
%calculateEntropyPartition
%   Input: partition
%   Output: entropy of partition

    entropy = 0;
    
    n = length(partition);
    K = max(partition);
    commNodeList = zeros(K,n);
    p = zeros(K,1);
    
    it = 1;
    for i=1:K
        for j=1:n
            if (partition(j) == i)
                commNodeList(i,it) = j;
                it = it + 1;
            end
        end
        it = 1;
        p(i) = nnz(commNodeList(i,:))/n;
    end
    
    for i=1:K
        entropy = entropy + (p(i) * log(p(i)));
    end
    
    entropy = -1*entropy;

end

