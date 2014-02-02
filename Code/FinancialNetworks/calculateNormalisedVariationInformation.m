function [ variationInformation ] = calculateNormalisedVariationInformation( partition1, partition2 )
%calculateNormalisedVariationInformation
%   Input: two partitions
%   Output: calculate normalised variation of information of two partitions

    n = length(partition1);

    entropyParition1 = calculateEntropyPartition(partition1);
    entropyParition2 = calculateEntropyPartition(partition2);
    mutualInformation = calculateMutualInformation(partition1,partition2);
    
    variationInformation = (entropyParition1 + entropyParition2 - (2*mutualInformation)) / log(n);

end