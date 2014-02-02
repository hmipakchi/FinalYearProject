function [ mutualInformation ] = calculateMutualInformation( partition1, partition2 )
%calculateMutualInformation
%   Input: two partitions
%   Output: calculate mutual information of two partitions

    mutualInformation = 0;
    
    n = length(partition1);
    
    K1 = max(partition1);
    K2 = max(partition2);
    
    commNodeList1 = zeros(K1,n);
    commNodeList2 = zeros(K2,n);
    
    p1 = zeros(K1,1);
    p2 = zeros(K2,1);
    pJoint = zeros(K1,K2);
    
    it = 1;
    for i=1:K1
        for j=1:n
            if (partition1(j) == i)
                commNodeList1(i,it) = j;
                it = it + 1;
            end
        end
        it = 1;
        p1(i) = nnz(commNodeList1(i,:))/n;
    end
    
    it = 1;
    for i=1:K2
        for j=1:n
            if (partition2(j) == i)
                commNodeList2(i,it) = j;
                it = it + 1;
            end
        end
        it = 1;
        p2(i) = nnz(commNodeList2(i,:))/n;
    end
    
    for i=1:K1
        for j=1:K2
            for l=1:n
                if (sum(ismember(commNodeList1(i,:),l)) == 1) && (sum(ismember(commNodeList2(j,:),l)) == 1)
                    pJoint(i,j) = pJoint(i,j) + 1;
                end
            end
        end
    end
    pJoint = pJoint/n;
    
    for i=1:K1
        for j=1:K2
            term = 0;
            if (pJoint(i,j) ~= 0)
                term = (pJoint(i,j) * log(pJoint(i,j)/(p1(i)*p2(j))));
            end
            mutualInformation = mutualInformation + term;
        end
    end

end