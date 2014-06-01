function [ standardisedPartition ] = standardisePartition( partition )
    n = length(partition);
    standardisedPartition = zeros(n,1);
    
    adjustment = min(unique(partition))-1;
    
    adjustedPartition = partition - (adjustment.*ones(n,1));
    uniqueAdjustedPartition = unique(adjustedPartition);
    
    for i=1:n
        for j=1:length(uniqueAdjustedPartition)
            if (adjustedPartition(i) == uniqueAdjustedPartition(j))
                standardisedPartition(i) = j;
            end
        end
    end
end