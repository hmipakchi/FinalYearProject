function [ variationInformationMatrix ] = calculatePairwiseNormalisedVariationInformationAsMatrix( communitiesMatrix )

    [noNodesPerCommunity,noAlgorithms] = size(communitiesMatrix);
    
    variationInformationMatrix = zeros(noAlgorithms,noAlgorithms);
    
    for i=1:noAlgorithms
        for j=1:noAlgorithms
            variationInformationMatrix(i,j) = calculateNormalisedVariationInformation(communitiesMatrix(:,i),communitiesMatrix(:,j));
        end
    end

end