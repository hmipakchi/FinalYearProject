clear;

n = 100;
noCommunities = 10;

timeSeriesCommunities = zeros(n, 1);
for i=1:n
    if (mod(i,noCommunities) == 0)
        timeSeriesCommunities(i,1) = i/noCommunities;
    else
        timeSeriesCommunities(i,1) = floor(i/noCommunities) + 1;
    end
end

% SNR between 0 and 1
SNR = [0.3; 0.8];

for s=1:length(SNR)
    s_str = num2str(s);
    
    % system wide correlations
    systemWide = zeros(noCommunities,noCommunities);
    for i=1:noCommunities
        for j=1:noCommunities
            if (mod(i,2) == 0) && mod(j,2) == 0
                systemWide(i,j) = sign((2*rand(1))-1)*rand(1)*rand(1);
            end
        end
    end
    systemWide = systemWide/2;

    correlationMatrix = zeros(n,n);
    for i=1:n
        for j=1:n
            if (timeSeriesCommunities(i) == timeSeriesCommunities(j))
                correlationMatrix(i,j) = SNR(s) + systemWide(timeSeriesCommunities(i),timeSeriesCommunities(j));
            else
                correlationMatrix(i,j) = 0 + systemWide(timeSeriesCommunities(i),timeSeriesCommunities(j));
            end
            if abs(correlationMatrix(i,j)) > 1
                correlationMatrix(i,j) = sign(correlationMatrix(i,j));
            end
        end
    end

    % plot synthetic correlation matrices
    filename = sprintf('../data_files/financialNetworks/syntheticCorrelationMatrices_%s.dat',s_str);
    fileID = fopen(filename,'w');
    for i=n:-1:1
        for j=1:n
            if j ~= n
                fprintf(fileID,'%d ',correlationMatrix(i,j));
            else
                fprintf(fileID,'%d',correlationMatrix(i,j));
            end
        end
        fprintf(fileID,'\n');
    end
    fclose(fileID);
end