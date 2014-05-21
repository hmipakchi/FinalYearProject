function [ communityAssignments ] = readFinancialSpectralClusteringCommunitiesFromFile( n )

filename_str = sprintf('../data_files/financialNetworks/financialSpectralClusteringOutputCommunities.dat');
fileID = fopen(filename_str,'r');
formatSpec = '%f';
communityAssignments = fscanf(fileID,formatSpec);
fclose(fileID);