function [] = writeGroupedCommunitiesByTickersToGexfFile( filename_str , communitiesVector, tickersVector)
    
    noStocks = length(communitiesVector);
    noCommunities = length(unique(communitiesVector));
    stockPositionInCommunity = zeros(noStocks,1);
    countMembersEachCommunity = zeros(noCommunities,1);
    
    for i=1:noStocks
        countMembersEachCommunity(communitiesVector(i)) = countMembersEachCommunity(communitiesVector(i)) + 1;
        stockPositionInCommunity(i) = countMembersEachCommunity(communitiesVector(i));
    end
    
    maxNoCommunitiesPerColumn = 2;
    maxNoNodesPerLine = 8;
    widthText = 150;
    heightText = 80;
    spaceConstant = 2;
    communityColumnSpacing = (maxNoNodesPerLine*widthText)+(spaceConstant*widthText);
    communityRowSpacing = (ceil(max(countMembersEachCommunity)/maxNoNodesPerLine)*heightText)+(spaceConstant*heightText);
    
    fileID = fopen(filename_str,'w');
    
    fprintf(fileID,'<gexf xmlns="http://www.gexf.net/1.2draft" xmlns:viz="http://www.gexf.net/1.1draft/viz" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:schemaLocation="http://www.gexf.net/1.2draft http://www.gexf.net/1.2draft/gexf.xsd" version="1.2"><graph defaultedgetype="undirected" idtype="string" type="static">\n');
    
    fprintf(fileID,'<attributes class="node"><attribute id="0" title="communitiy" type="float"/></attributes>\n');
    
    fprintf(fileID,'<nodes count="%d">\n',noStocks);
    for i=1:noStocks
        communityOfStock = communitiesVector(i);
        communityPositionOfStock = stockPositionInCommunity(i);
        xPosCommunityContribution = mod((communityOfStock-1),maxNoCommunitiesPerColumn) * communityColumnSpacing;
        yPosCommunityContribution = -1 * floor((communityOfStock-1)/maxNoCommunitiesPerColumn) * communityRowSpacing;
        xPosNodeContribution = mod((communityPositionOfStock-1),maxNoNodesPerLine) * widthText;
        yPosNodeContribution = -1 * floor((communityPositionOfStock-1)/maxNoNodesPerLine) * heightText;
        xPos = xPosCommunityContribution + xPosNodeContribution;
        yPos = yPosCommunityContribution + yPosNodeContribution;
        fprintf(fileID,'<node id="%d" label="%s">',i,tickersVector{1,1}{i});
        fprintf(fileID,'<attvalues><attvalue for="0" value="%d"/></attvalues>',communityOfStock);
        fprintf(fileID,'<viz:position x="%d" y="%d" z="0"/>',xPos,yPos);
        fprintf(fileID,'</node>\n');
    end
    fprintf(fileID,'</nodes>\n');
    fprintf(fileID,'</graph>\n');
    fprintf(fileID,'</gexf>');

    fclose(fileID);
end