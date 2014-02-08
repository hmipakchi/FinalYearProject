function [] = writeGroupedCommunitiesByIndustriesToGexfFile( filename_str , communitiesVector, ICBIndustriesVector)

    noStocks = length(communitiesVector);
    noCommunities = length(unique(communitiesVector));
    noIndustries = 10;
    countMembersEachIndustryInCommunities = zeros(noCommunities,noIndustries);
    
    for i=1:noStocks
        countMembersEachIndustryInCommunities(communitiesVector(i),ICBIndustriesVector{1,1}(i)) = countMembersEachIndustryInCommunities(communitiesVector(i),ICBIndustriesVector{1,1}(i)) + 1;
    end
    
    fileID = fopen(filename_str,'w');
    
    fprintf(fileID,'<gexf xmlns="http://www.gexf.net/1.2draft" xmlns:viz="http://www.gexf.net/1.1draft/viz" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:schemaLocation="http://www.gexf.net/1.2draft http://www.gexf.net/1.2draft/gexf.xsd" version="1.2"><graph defaultedgetype="undirected" idtype="string" type="static">\n');
    
    fprintf(fileID,'<attributes class="node">');
    for i=1:noIndustries
        fprintf(fileID,'<attribute id="%d" title="composition_of_%s" type="float"/>',i,decodeIndustryId(i));
    end
    fprintf(fileID,'<attribute id="%d" title="size_of_community" type="float"/>',noIndustries+1);
    fprintf(fileID,'</attributes>\n');
    
    fprintf(fileID,'<nodes count="%d">\n',noCommunities);
    for i=1:noCommunities
        noMembersInCommunity = sum(countMembersEachIndustryInCommunities(i,:));
        fprintf(fileID,'<node id="%d">',i);
        fprintf(fileID,'<attvalues>');
        for j=1:noIndustries
            composition = countMembersEachIndustryInCommunities(i,j) / noMembersInCommunity;
            fprintf(fileID,'<attvalue for="%d" value="%.3f"/>',j,composition);
        end
        fprintf(fileID,'<attvalue for="%d" value="%d"/>',noIndustries+1,noMembersInCommunity);
        fprintf(fileID,'</attvalues>');
        fprintf(fileID,'</node>\n');
    end
    fprintf(fileID,'</nodes>\n');
    fprintf(fileID,'</graph>\n');
    fprintf(fileID,'</gexf>');

    fclose(fileID);

end
    
function [ industry ] = decodeIndustryId( i )
    if i == 1
        industry = 'Oil and Gas';
    elseif i == 2
        industry = 'Basic Materials';
    elseif i == 3
        industry = 'Industrials';
    elseif i == 4
        industry = 'Consumer Goods';
    elseif i == 5
        industry = 'Health Care';
    elseif i == 6
        industry = 'Consumer Services';
    elseif i == 7
        industry = 'Telecommunications';
    elseif i == 8
        industry = 'Utilities';
    elseif i == 9
        industry = 'Financials';
    elseif i == 10
        industry = 'Technology';
    else
        industry =  '';
    end    
end