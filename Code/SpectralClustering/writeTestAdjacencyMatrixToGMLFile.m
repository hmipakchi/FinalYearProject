function [ ] = writeTestAdjacencyMatrixToGMLFile( gmlOutputFileName_str, adjacencyMatrix, nodeCommunities )

    fileID = fopen(gmlOutputFileName_str,'w');
    fprintf(fileID,'graph [\n');
    
    [noRows,noCols] = size(adjacencyMatrix);
    
    for i=1:noRows
        fprintf(fileID,'  node\n');
        fprintf(fileID,'  [\n');
        fprintf(fileID,'    id %d\n', i-1);
%         fprintf(fileID,'    value %d\n', nodeCommunities(i)-1);
        fprintf(fileID,'    value %d\n', 0);
        fprintf(fileID,'  ]\n');
    end
    
    for i=1:noRows
        for j=1:noCols
            if (adjacencyMatrix(i,j) == 1)
                fprintf(fileID,'  edge\n');
                fprintf(fileID,'  [\n');
                fprintf(fileID,'    source %d\n', i-1);
                fprintf(fileID,'    target %d\n', j-1);
                fprintf(fileID,'  ]\n');
            end
        end
    end
    
    fclose(fileID);
end

