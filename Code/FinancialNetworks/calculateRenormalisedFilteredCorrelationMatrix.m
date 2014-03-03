function [ renormalisedFilteredCorrelationMatrix ] = calculateRenormalisedFilteredCorrelationMatrix( modularityMatrix, communities )
    noCommunities = length(unique(communities));
    renormalisedFilteredCorrelationMatrix = zeros(noCommunities,noCommunities);
    noStocks = length(communities);
    
    for i=1:noStocks
        for j=1:noStocks
            comI = communities(i);
            comJ = communities(j);
            renormalisedFilteredCorrelationMatrix(comI,comJ) = renormalisedFilteredCorrelationMatrix(comI,comJ) + modularityMatrix(i,j); 
        end
    end
    
    if (noStocks ~= 0)
        renormalisedFilteredCorrelationMatrix = (1/noStocks).*renormalisedFilteredCorrelationMatrix;
    end
end

