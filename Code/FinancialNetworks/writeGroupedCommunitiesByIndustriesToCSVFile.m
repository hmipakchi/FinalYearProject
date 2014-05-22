function [] = writeGroupedCommunitiesByIndustriesToCSVFile( filename_str , communitiesVector, ICBIndustriesVector)

    noStocks = length(communitiesVector);
    noCommunities = length(unique(communitiesVector));
    noIndustries = 10;
    countMembersEachIndustryInCommunities = zeros(noCommunities,noIndustries);
    
    for i=1:noStocks
        countMembersEachIndustryInCommunities(communitiesVector(i),ICBIndustriesVector{1,1}(i)) = countMembersEachIndustryInCommunities(communitiesVector(i),ICBIndustriesVector{1,1}(i)) + 1;
    end
    
    fileID = fopen(filename_str,'w');
    
    for i=1:noCommunities
        fprintf(fileID,'Industry,Composition of Industry\n');
        noMembersInCommunity = sum(countMembersEachIndustryInCommunities(i,:));
        for j=1:noIndustries
            composition = countMembersEachIndustryInCommunities(i,j) / noMembersInCommunity;
            fprintf(fileID,'%s,%.3f\n',decodeIndustryId(j),composition);
        end
        fprintf(fileID,'\n\n');
    end
    fclose(fileID);

end