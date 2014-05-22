function [] = writeRenormalisedFilteredCorrelationsMatrixToFile( filename_str , renormalisedFilteredCorrelation)
    fileID = fopen(filename_str,'w');
    [noRows,noCols] = size(renormalisedFilteredCorrelation);
    for i=1:noRows
        for j=1:noCols
            fprintf(fileID,'%d ',renormalisedFilteredCorrelation(i,j));
        end
        fprintf(fileID,'\n');
    end
    fclose(fileID);

end