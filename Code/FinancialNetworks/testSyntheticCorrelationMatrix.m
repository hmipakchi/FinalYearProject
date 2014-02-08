clear;

n = 100;
noCommunities = 10;

timeSeriesCommunities = zeros(n,1);
count = 1;
for i=1:n
    timeSeriesCommunities(i) = count;
    if (mod(i,n/noCommunities)==0)
        count = count + 1;
    end
end

% SNR varies between 0 and 1
% SNR = [0.1; 0.2; 0.3; 0.4; 0.5; 0.6; 0.7; 0.8; 0.9; 1.0;];
SNR = [0.9;];

fastNewmanCommunities = zeros(n,length(SNR));
fastNewmanModularities = zeros(length(SNR),1);
fastNewmanVariationalInformation = zeros(length(SNR),1);

fastNewmanModifiedCommunities = zeros(n,length(SNR));
fastNewmanModifiedModularities = zeros(length(SNR),1);
fastNewmanModifiedVariationalInformation = zeros(length(SNR),1);

for s=1:length(SNR)
    s_str = num2str(s);
    
    % system wide correlations
    systemWide = zeros(noCommunities,noCommunities);
    for i=1:noCommunities
        for j=1:noCommunities
            if (mod(i,2) == 0) && mod(j,2) == 0
                systemWide(i,j) = sign((2*rand(1))-1)*0.2*rand(1);
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
%     filename = sprintf('../data_files/financialNetworks/syntheticCorrelationMatrices_%s.dat',s_str);
%     fileID = fopen(filename,'w');
%     for i=1:n
%         for j=1:n
%             fprintf(fileID,'%d ',correlationMatrix(i,j));
%         end
%         fprintf(fileID,'\n');
%     end
%     fclose(fileID);

    % use synthetic correlation matrices to form weighted adjacency matrix
%     filename = sprintf('../data_files/financialNetworks/syntheticWeightedAdjacencyMatrices_%s.dat',s_str);
%     fileID = fopen(filename,'w');
%     for i=1:n
%         for j=1:n
%             value = 0.5*(correlationMatrix(i,j)+1)-delta(i,j);
% %             value = correlationMatrix(i,j);
%             fprintf(fileID,'%d %d %d',i,j,value);
%             fprintf(fileID,'\n');
%         end
%     end
%     fclose(fileID);
    
    weightedAdjacencyMatrix = zeros(n,n);
    weightedAdjacencyMatrixModified = zeros(n,n);
    for i=1:n
        for j=1:n
%             weightedAdjacencyMatrix(i,j) = correlationMatrix(i,j);
            weightedAdjacencyMatrix(i,j) = 0.5*(correlationMatrix(i,j)+1) - delta(i,j);
        end
    end
    
    [fastNewmanCommunities(:,s),fastNewmanModularities(s)] = fast_newman(weightedAdjacencyMatrix);
    fastNewmanVariationalInformation(s) = calculateNormalisedVariationInformation(timeSeriesCommunities,fastNewmanCommunities(:,s));
    
    [fastNewmanModifiedCommunities(:,s),fastNewmanModifiedModularities(s)] = fast_newman_modified(weightedAdjacencyMatrixModified);
    fastNewmanModifiedVariationalInformation(s) = calculateNormalisedVariationInformation(timeSeriesCommunities,fastNewmanModifiedCommunities(:,s));
    
    % plot density of eigenvalues of sample cross-correlation matrix
    eigenvalues = [];
    % Take real part to compensate for numerical issues
    eigenvalues = [eigenvalues; real(eig(correlationMatrix))];

    maxEmpiricalEigenvalue = max(eigenvalues);
    minEmpiricalEigenvalue = min(eigenvalues);
    noEigenvalues = 1000;
    eigenvalueStep = (maxEmpiricalEigenvalue - minEmpiricalEigenvalue) / noEigenvalues;

    theoreticalSpectrum = zeros(noEigenvalues,2);
    it = 1;
    eigenvalue = minEmpiricalEigenvalue;
    while eigenvalue <= maxEmpiricalEigenvalue
        it = it + 1;
        eigenvalue = eigenvalue + eigenvalueStep;
    end

    nbins = 200;
    [spectrum,lambda] = hist(eigenvalues,nbins);
    plot(lambda,spectrum/trapz(lambda,spectrum),'red','LineWidth',2);
    hold on;
    
end