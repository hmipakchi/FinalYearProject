clear;

T = 2500;
n = 100;
noCommunities = 10;

means = zeros(n,1);
standardDeviations = ones(n,1);

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
SNR = [0.5; 0.7; 1.0;];

fastNewmanCommunities = zeros(n,length(SNR));
fastNewmanModularities = zeros(length(SNR),1);
fastNewmanVariationalInformation = zeros(length(SNR),1);

fastNewmanModifiedCommunities = zeros(n,length(SNR));
fastNewmanModifiedModularities = zeros(length(SNR),1);
fastNewmanModifiedVariationalInformation = zeros(length(SNR),1);

montanariCommunities = zeros(n,length(SNR));
montanariVariationalInformation = zeros(length(SNR),1);

spectralClusteringCommunities = zeros(n,length(SNR));
spectralClusteringVariationalInformation = zeros(length(SNR),1);

for s=1:length(SNR)
    s_str = num2str(s);
    
    % system wide correlations
    systemWide = zeros(noCommunities,noCommunities);
    for i=1:noCommunities
        for j=1:noCommunities
            if (mod(i,2) == 0) && mod(j,2) == 0
                systemWide(i,j) = abs(sign((2*rand(1))-1))*0.1;
            end
        end
    end

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
    
    timeSeriesData = mvnrnd(means,correlationMatrix,T);
%     sampleCrossCorrelationMatrix = correlationMatrix;
    sampleCrossCorrelationMatrix = calculateSampleCrossCorrelationMatrix(timeSeriesData');

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
    for i=1:n
        for j=1:n
%             weightedAdjacencyMatrix(i,j) = correlationMatrix(i,j);
            weightedAdjacencyMatrix(i,j) = 0.5*(sampleCrossCorrelationMatrix(i,j)+1) - delta(i,j);
        end
    end
    
    % plot density of eigenvalues of sample cross-correlation matrix
    eigenvalues = [];
    % Take real part to compensate for numerical issues
    eigenvalues = [eigenvalues; real(eig(sampleCrossCorrelationMatrix))];

    maxEmpiricalEigenvalue = max(eigenvalues);
    minEmpiricalEigenvalue = min(eigenvalues);
    maxTheoreticalEigenvalue = (1 + sqrt(n/T))^2;
    minTheoreticalEigenvalue = (1 - sqrt(n/T))^2;
    noEigenvalues = 1000;
    eigenvalueStep = (maxEmpiricalEigenvalue - minEmpiricalEigenvalue) / noEigenvalues;

    theoreticalSpectrum = zeros(noEigenvalues,2);
    it = 1;
    eigenvalue = minEmpiricalEigenvalue;
    while eigenvalue <= maxEmpiricalEigenvalue
        theoreticalSpectrum(it,1) = eigenvalue;
        if (minTheoreticalEigenvalue <= eigenvalue) && (eigenvalue <= maxTheoreticalEigenvalue)
            theoreticalSpectrum(it,2) = (T/(2*pi*n*eigenvalue))*sqrt((maxTheoreticalEigenvalue - eigenvalue)*(eigenvalue - minTheoreticalEigenvalue));
        else
            theoreticalSpectrum(it,2) = 0;
        end
        it = it + 1;
        eigenvalue = eigenvalue + eigenvalueStep;
    end
    
    nbins = 200;
    [spectrum,lambda] = hist(eigenvalues,nbins);
    plot(lambda,spectrum/trapz(lambda,spectrum),'red','LineWidth',2);
    hold on;

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    
    [V,D] = eig(sampleCrossCorrelationMatrix);
    modularityMatrix = zeros(n,n);
    for i=1:n
        if (eigenvalues(i) > maxTheoreticalEigenvalue)% && (eigenvalues(i) < maxEmpiricalEigenvalue)
            disp(i);
            modularityMatrix = modularityMatrix + (eigenvalues(i).*(V(:,i)*V(:,i)'));
        end
    end

    [fastNewmanCommunities(:,s),fastNewmanModularities(s)] = fast_newman(weightedAdjacencyMatrix);
%     fastNewmanVariationalInformation(s) = calculateNormalisedVariationInformation(timeSeriesCommunities,fastNewmanCommunities(:,s));

    [fastNewmanModifiedCommunities(:,s),fastNewmanModifiedModularities(s)] = fast_newman_modified(modularityMatrix);
%     fastNewmanModifiedVariationalInformation(s) = calculateNormalisedVariationInformation(timeSeriesCommunities,fastNewmanModifiedCommunities(:,s));

    [montanariCommunities(:,s)] = montanari_modularity(weightedAdjacencyMatrix);
%     montanariVariationalInformation(s) = calculateNormalisedVariationInformation(timeSeriesCommunities,montanariCommunities(:,s));

    [spectralClusteringCommunities(:,s)] = financialSpectralClustering(modularityMatrix, noCommunities);
%     spectralClusteringVariationalInformation(s) = calculateNormalisedVariationInformation(timeSeriesCommunities,spectralClusteringCommunities(:,s));
    
end