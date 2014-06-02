clear;

T = 2500;
n = 100;
noCommunities = 10;%10

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

SNR = [0.5;0.6;0.7;0.8;];

normalisedVariationInformationTemporalEvolutionTensor = cell(length(SNR),1);
testMultiSliceLouvainMethodModularitiesTensor = cell(length(SNR),1);

for SNRIterator=1:length(SNR)
    s_str = num2str(SNRIterator);
    
    currentSNR = SNR(SNRIterator);
    display(currentSNR);
    
    systemWide = zeros(noCommunities,noCommunities);
    for i=1:noCommunities
        for j=1:noCommunities
            systemWide(i,j) = -1*abs(rand(1))*0.1;
            if (i > j)
                systemWide(i,j) = systemWide(j,i);
            end
        end
    end

    correlationMatrix = zeros(n,n);
    for i=1:n
        for j=1:n
            if (timeSeriesCommunities(i) == timeSeriesCommunities(j))
                correlationMatrix(i,j) = SNR(SNRIterator) + systemWide(timeSeriesCommunities(i),timeSeriesCommunities(j));
            elseif (mod(timeSeriesCommunities(i),2) == 0) && (mod(timeSeriesCommunities(j),2) == 0)
                correlationMatrix(i,j) = 0 + systemWide(timeSeriesCommunities(i),timeSeriesCommunities(j));
            else
               correlationMatrix(i,j) = 0 - systemWide(timeSeriesCommunities(i),timeSeriesCommunities(j));
            end
            if abs(correlationMatrix(i,j)) > 1
                correlationMatrix(i,j) = sign(correlationMatrix(i,j));
            end
        end
    end
    
    timeSeriesData = transpose(mvnrnd(means,correlationMatrix,T));
    
    lengthTimeWindow = 100;
    rolloverPeriod = 10;
    noRolloverTimeWindows = floor((T-lengthTimeWindow)/rolloverPeriod);
    timeIndexMatrix = zeros(1,floor((T-lengthTimeWindow)/rolloverPeriod));
    
    tensorModularityMatrices = cell(noRolloverTimeWindows,1);
    testLouvainMethodTemporalCommunities = zeros(n,noRolloverTimeWindows);
    
    for j=1:noRolloverTimeWindows
        
        windowStartIndex = ((j-1)*rolloverPeriod) + 1;
        windowsEndIndex = windowStartIndex + lengthTimeWindow;
        timeIndexMatrix(1,j) = windowsEndIndex;
        
        windowedLogReturns = timeSeriesData(:,windowStartIndex:windowsEndIndex);

        % calculate calculate time average and sample variance of windowed logarithmic returns
        timeAverage = calculateTimeAverage(windowedLogReturns);
        meanSquareTimeAverage = calculateMeanSquareTimeAverage(windowedLogReturns);
        sampleVariance = calculateSampleVariance(windowedLogReturns);

        % calculate filtered windowed logarithmic returns
        windowedLogReturns = calculateFilteredLogarithmicReturns(windowedLogReturns);

        % calculate sample cross-correlation matrix of filtered windowed logarithmic returns
        meanProductTimeAverage = calculateMeanProductTimeAverage(windowedLogReturns);
        sampleWindowedCrossCorrelationMatrix = calculateSampleCrossCorrelationMatrix(windowedLogReturns);

        eigenvalues = [];
        eigenvalues = [eigenvalues; real(eig(sampleWindowedCrossCorrelationMatrix))];
        maxEmpiricalEigenvalue = max(eigenvalues);
        minEmpiricalEigenvalue = min(eigenvalues);
        maxTheoreticalEigenvalue = (1 + sqrt(n/noRolloverTimeWindows))^2;
        minTheoreticalEigenvalue = (1 - sqrt(n/noRolloverTimeWindows))^2;
        noEigenvalues = 1000;
        eigenvalueStep = (maxEmpiricalEigenvalue - minEmpiricalEigenvalue) / noEigenvalues;
        theoreticalSpectrum = zeros(noEigenvalues,2);
        it = 1;
        eigenvalue = minEmpiricalEigenvalue;
        while eigenvalue <= maxEmpiricalEigenvalue
            theoreticalSpectrum(it,1) = eigenvalue;
            if (minTheoreticalEigenvalue <= eigenvalue) && (eigenvalue <= maxTheoreticalEigenvalue)
                theoreticalSpectrum(it,2) = (noRolloverTimeWindows/(2*pi*n*eigenvalue))*sqrt((maxTheoreticalEigenvalue - eigenvalue)*(eigenvalue - minTheoreticalEigenvalue));
            else
                theoreticalSpectrum(it,2) = 0;
            end
            it = it + 1;
            eigenvalue = eigenvalue + eigenvalueStep;
        end
        [V,D] = eig(sampleWindowedCrossCorrelationMatrix);
        modularityMatrix = zeros(n,n);
        for p=1:n
            if (eigenvalues(p) > maxTheoreticalEigenvalue)% && (eigenvalues(p) < maxEmpiricalEigenvalue)
                modularityMatrix = modularityMatrix + (eigenvalues(p).*(V(:,p)*V(:,p)'));
            end
        end
        
        limit = 10000;
        k = full(sum(modularityMatrix));
        twom = sum(k);
        B = @(v) modularityMatrix(:,v) - k'*k(v)/twom;
        testLouvainMethodTemporalCommunities(:,j) = genlouvain(B,limit,0);
        
        tensorModularityMatrices(j) = {modularityMatrix};
    end
    
    gamma = 1;
    omegaVector = [0.25; 0.5; 0.75; 1;];%0.25 or 0.5 or 0.75 or 1
    testMultiSliceLouvainMethodCommunities = cell(length(omegaVector),1);
    testMultiSliceLouvainMethodModularities = zeros(length(omegaVector),1);
    
    for omegaIterator = 1:length(omegaVector)
        omega = omegaVector(omegaIterator);
        display(omega);
        ii=[]; jj=[]; vv=[];
        twomu=0;
        for s=1:noRolloverTimeWindows
            indx=[1:n]'+(s-1)*n;
            [i,j,v]=find(tensorModularityMatrices{s});
            ii=[ii;indx(i)]; jj=[jj;indx(j)]; vv=[vv;v];
            k=sum(tensorModularityMatrices{s});
            kv=zeros(n*noRolloverTimeWindows,1);
            twom=sum(k);
            twomu=twomu+twom;
            kv(indx)=k/twom;
            kcell{s}=kv;
        end
        AA = sparse(ii,jj,vv,n*noRolloverTimeWindows,n*noRolloverTimeWindows);
        clear ii jj vv
        kvec = full(sum(AA));
        AA = AA + omega*spdiags(ones(n*noRolloverTimeWindows,2),[-n,n],n*noRolloverTimeWindows,n*noRolloverTimeWindows);
        twomu=twomu+2*omega*n*(noRolloverTimeWindows-1);
        B = @(i) AA(:,i) - gamma*kcell{ceil(i/(n+eps))}*kvec(i);
        [S,Q] = genlouvain(B);
        Q = Q/twomu
        S = reshape(S,n,noRolloverTimeWindows);
        testMultiSliceLouvainMethodCommunities(omegaIterator) = {S};
        testMultiSliceLouvainMethodModularities(omegaIterator) = Q;
    end
    
    testMultiSliceLouvainMethodModularitiesTensor(SNRIterator) = {testMultiSliceLouvainMethodModularities};
    
    normalisedVariationInformationTemporalEvolutionMatrix = zeros(noRolloverTimeWindows,length(omegaVector)+1);
    for t = 1:noRolloverTimeWindows
%         display(t);
        for omegaIterator=1:length(omegaVector)
            normalisedVariationInformationTemporalEvolutionMatrix(t,omegaIterator) = calculateNormalisedVariationInformation(timeSeriesCommunities,testMultiSliceLouvainMethodCommunities{omegaIterator,1}(:,t));
        end
        normalisedVariationInformationTemporalEvolutionMatrix(t,length(omegaVector)+1) = calculateNormalisedVariationInformation(timeSeriesCommunities,testLouvainMethodTemporalCommunities(:,t));
    end
    
    normalisedVariationInformationTemporalEvolutionTensor(SNRIterator) = {normalisedVariationInformationTemporalEvolutionMatrix};
end

normalisedVariationInformationTemporalEvolutionTensorAverages = zeros(noRolloverTimeWindows,length(omegaVector)+1);

for omegaIteration=1:length(omegaVector)
    for SNRIterator=1:length(SNR)
        normalisedVariationInformationTemporalEvolutionTensorAverages(:,omegaIteration) = normalisedVariationInformationTemporalEvolutionTensorAverages(:,omegaIteration) + normalisedVariationInformationTemporalEvolutionTensor{SNRIterator,1}(:,omegaIteration);
    end
    normalisedVariationInformationTemporalEvolutionTensorAverages(:,omegaIteration) = normalisedVariationInformationTemporalEvolutionTensorAverages(:,omegaIteration) / length(SNR);
end
for SNRIterator=1:length(SNR)
    normalisedVariationInformationTemporalEvolutionTensorAverages(:,length(omegaVector)+1) = normalisedVariationInformationTemporalEvolutionTensorAverages(:,length(omegaVector)+1) + normalisedVariationInformationTemporalEvolutionTensor{SNRIterator,1}(:,length(omegaVector)+1);
end
normalisedVariationInformationTemporalEvolutionTensorAverages(:,length(omegaVector)+1) = normalisedVariationInformationTemporalEvolutionTensorAverages(:,length(omegaVector)+1) / length(SNR);

    
% write averaged normalisedVariationOfInformation over all SNR between ground truth and MultiSliceLouvain methods (different omega)
filename_str = sprintf('../data_files/financialNetworks/averagedNormalisedVariationOfInformationTemporalSyntheticData.dat');
timeWindowLengthIndex = 1;
dateFormat = 'yyyy-mm-dd';
fileID = fopen(filename_str,'w');
for j=1:lengthTimeWindow
    fprintf(fileID,'%d ',j);
    for k=1:length(omegaVector)+1
        fprintf(fileID,'%f ',normalisedVariationInformationTemporalEvolutionTensorAverages(j,k));
    end
    fprintf(fileID,'\n');
end
fclose(fileID);

% write Modularities over all SNR for MultiSliceLouvain methods (different omega)
filename_str = sprintf('../data_files/financialNetworks/testMultiSliceLoucainMethodModularitiesSyntheticData.dat');
timeWindowLengthIndex = 1;
dateFormat = 'yyyy-mm-dd';
fileID = fopen(filename_str,'w');
for SNRIterator=1:length(SNR)
    fprintf(fileID,'%d %f ',SNRIterator,SNR(SNRIterator));
    for k=1:length(omegaVector)
        fprintf(fileID,'%f ',testMultiSliceLouvainMethodModularitiesTensor{SNRIterator,1}(k,1));
    end
    fprintf(fileID,'\n');
end
fclose(fileID);