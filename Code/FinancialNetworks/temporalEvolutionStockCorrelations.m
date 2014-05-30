clear;

% S&P 500 Data
% load('stockdata.mat');
% priceMatrix = price;

% FTSE 100 Data, tickers and tested dates
FTSEStockDataFilename = 'outputStockData.txt';
priceMatrix = dlmread(FTSEStockDataFilename);
tickersFilename = 'tickers.txt';
fileID = fopen(tickersFilename);
tickersVector = textscan(fileID,'%s');
fclose(fileID);
testableDatesFilename = 'testableDates.txt';
fileID = fopen(testableDatesFilename);
testableDatesVector = textscan(fileID,'%s');
fclose(fileID);
ICBIndustriesFilename = 'ICBIndustries.txt';
fileID = fopen(ICBIndustriesFilename);
ICBIndustriesVector = textscan(fileID,'%d');
fclose(fileID);

% remove stocks with strange prices - need to sort out!!!
rowsToDelete = [11;25;36;64;65;76;79];
for candidateRow=1:size(rowsToDelete,1)
    priceMatrix(rowsToDelete(candidateRow)-candidateRow+1, :) = [];
%     ICBIndustriesVector{rowsToDelete(candidateRow)-candidateRow+1,1} = [];
    tickersVector{1,1}(rowsToDelete(candidateRow)) = [];
end

[n,T] = size(priceMatrix);

n_str = num2str(n);
T_str = num2str(T);

% calculate logarithmic returns for stock price data
logReturns = calculateLogarithmicReturns(priceMatrix);

% lengthTimeWindowVector = [100;150;200]; % uncomment for testing
lengthTimeWindowVector = [100;];

nolengthTimeWindows = length(lengthTimeWindowVector);
rolloverPeriod = 10;%10 or 1
rolloverPeriod_str = num2str(rolloverPeriod);

noRolloverTimeWindowsPerLengthtimeWindows = zeros(nolengthTimeWindows,1);

timeIndexMatrix = zeros(nolengthTimeWindows,floor((T-min(lengthTimeWindowVector))/rolloverPeriod));

meanCorrelationCoefficientsMatrix = -100.*ones(nolengthTimeWindows,floor((T-min(lengthTimeWindowVector))/rolloverPeriod));
varianceCorrelationCoefficientsMatrix = -100.*ones(nolengthTimeWindows,floor((T-min(lengthTimeWindowVector))/rolloverPeriod));
skewnessCorrelationCoefficientsMatrix = -100.*ones(nolengthTimeWindows,floor((T-min(lengthTimeWindowVector))/rolloverPeriod));

numberPCAnalysed = 12;%5

for i=1:nolengthTimeWindows
    
    lengthTimeWindow = lengthTimeWindowVector(i);
    noRolloverTimeWindows = floor((T-lengthTimeWindow)/rolloverPeriod);
    noRolloverTimeWindowsPerLengthtimeWindows(i,1) = noRolloverTimeWindows;
    
    eigenvalueContributionPCMatrix = zeros(noRolloverTimeWindowsPerLengthtimeWindows(1),numberPCAnalysed);
    
    tensorCorrelationMatrices = cell(noRolloverTimeWindows,1);
    tensorModularityMatrices = cell(noRolloverTimeWindows,1);
    
    testLouvainMethodTemporalCommunities = zeros(n,noRolloverTimeWindows);
    
    for j=1:noRolloverTimeWindows
        
        windowStartIndex = ((j-1)*rolloverPeriod) + 1;
        windowsEndIndex = windowStartIndex + lengthTimeWindow;
        timeIndexMatrix(i,j) = windowsEndIndex;
        
        windowedLogReturns = logReturns(:,windowStartIndex:windowsEndIndex);

        % calculate calculate time average and sample variance of windowed logarithmic returns
        timeAverage = calculateTimeAverage(windowedLogReturns);
        meanSquareTimeAverage = calculateMeanSquareTimeAverage(windowedLogReturns);
        sampleVariance = calculateSampleVariance(windowedLogReturns);

        % calculate filtered windowed logarithmic returns
        windowedLogReturns = calculateFilteredLogarithmicReturns(windowedLogReturns);

        % calculate sample cross-correlation matrix of filtered windowed logarithmic returns
        meanProductTimeAverage = calculateMeanProductTimeAverage(windowedLogReturns);
        sampleCrossCorrelationMatrix = calculateSampleCrossCorrelationMatrix(windowedLogReturns);
        
        % calculate mean, variance and skewness of correlation coefficients
%         meanCorrelationCoefficientsMatrix(i,j) = calculateMeanCorrelationValue(sampleCrossCorrelationMatrix);
%         varianceCorrelationCoefficientsMatrix(i,j) = calculateVarianceCorrelationValue(sampleCrossCorrelationMatrix);
%         skewnessCorrelationCoefficientsMatrix(i,j) = calculateSkewnessCorrelationValue(sampleCrossCorrelationMatrix);

%         [eigenvalues] = eig(sampleCrossCorrelationMatrix);
%         eigenvalues = sort(eigenvalues,'descend');
%         for k=1:numberPCAnalysed
%             eigenvalueContributionPCMatrix(j,k) = eigenvalues(k)/n;
%         end

        eigenvalues = [];
        eigenvalues = [eigenvalues; real(eig(sampleCrossCorrelationMatrix))];
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
        [V,D] = eig(sampleCrossCorrelationMatrix);
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
        
        tensorCorrelationMatrices(j) = {sampleCrossCorrelationMatrix};
        tensorModularityMatrices(j) = {modularityMatrix};
    end
    
%     gamma = 1;
%     omega = 0.5;
%     ii=[]; jj=[]; vv=[];
%     twomu=0;
%     for s=1:noRolloverTimeWindows
%         indx=[1:n]'+(s-1)*n;
%         [i,j,v]=find(tensorModularityMatrices{s});
%         ii=[ii;indx(i)]; jj=[jj;indx(j)]; vv=[vv;v];
%         k=sum(tensorModularityMatrices{s});
%         kv=zeros(n*noRolloverTimeWindows,1);
%         twom=sum(k);
%         twomu=twomu+twom;
%         kv(indx)=k/twom;
%         kcell{s}=kv;
%     end
%     AA = sparse(ii,jj,vv,n*noRolloverTimeWindows,n*noRolloverTimeWindows);
%     clear ii jj vv
%     kvec = full(sum(AA));
%     AA = AA + omega*spdiags(ones(n*noRolloverTimeWindows,2),[-n,n],n*noRolloverTimeWindows,n*noRolloverTimeWindows);
%     twomu=twomu+2*omega*n*(noRolloverTimeWindows-1);
%     B = @(i) AA(:,i) - gamma*kcell{ceil(i/(n+eps))}*kvec(i);
%     [S,Q] = genlouvain(B);
%     Q = Q/twomu
%     S = reshape(S,n,noRolloverTimeWindows);
end

% % write mean, variance and skewness coeffients to file
% dateFormat = 'yyyy-mm-dd';
% 
% % write mean correlation coefficients to file
% filename_str = sprintf('../data_files/financialNetworks/meanCorrelationCoefficientsRollover_%s.dat',rolloverPeriod_str);
% fileID = fopen(filename_str,'w');
% for j=1:max(noRolloverTimeWindowsPerLengthtimeWindows)
%     for i=1:nolengthTimeWindows
%         if (j <= noRolloverTimeWindowsPerLengthtimeWindows(i))
%             fprintf(fileID,'%s %d ',datestr(testableDatesVector{1,1}{timeIndexMatrix(i,j)},dateFormat),meanCorrelationCoefficientsMatrix(i,j));
%         end
%     end
%     fprintf(fileID,'\n');
% end
% fclose(fileID);
% 
% % write variance correlation coefficients to file
% filename_str = sprintf('../data_files/financialNetworks/varianceCorrelationCoefficientsRollover_%s.dat',rolloverPeriod_str);
% fileID = fopen(filename_str,'w');
% for j=1:max(noRolloverTimeWindowsPerLengthtimeWindows)
%     for i=1:nolengthTimeWindows
%         if (j <= noRolloverTimeWindowsPerLengthtimeWindows(i))
%             fprintf(fileID,'%s %d ',datestr(testableDatesVector{1,1}{timeIndexMatrix(i,j)},dateFormat),varianceCorrelationCoefficientsMatrix(i,j));
%         end
%     end
%     fprintf(fileID,'\n');
% end
% fclose(fileID);
% 
% % write skewness correlation coefficients to file
% filename_str = sprintf('../data_files/financialNetworks/skewnessCorrelationCoefficientsRollover_%s.dat',rolloverPeriod_str);
% fileID = fopen(filename_str,'w');
% for j=1:max(noRolloverTimeWindowsPerLengthtimeWindows)
%     for i=1:nolengthTimeWindows
%         if (j <= noRolloverTimeWindowsPerLengthtimeWindows(i))
%             fprintf(fileID,'%s %d ',datestr(testableDatesVector{1,1}{timeIndexMatrix(i,j)},dateFormat),skewnessCorrelationCoefficientsMatrix(i,j));
%         end
%     end
%     fprintf(fileID,'\n');
% end
% fclose(fileID);

% % write eigenvalue contributions of kth PC to file
% filename_str = sprintf('../data_files/financialNetworks/eigenvalueContributionsPC.dat');
% timeWindowLengthIndex = 1;
% dateFormat = 'yyyy-mm-dd';
% fileID = fopen(filename_str,'w');
% for j=1:noRolloverTimeWindowsPerLengthtimeWindows(timeWindowLengthIndex)
%     fprintf(fileID,'%s ',datestr(testableDatesVector{1,1}{timeIndexMatrix(timeWindowLengthIndex,j)},dateFormat));
%     for k=1:numberPCAnalysed
%         fprintf(fileID,'%d ',eigenvalueContributionPCMatrix(j,k));
%     end
%     fprintf(fileID,'\n');
% end
% fclose(fileID);

% write testableDatesYearOnly to file: for plot only!!!
filename_str = sprintf('../data_files/financialNetworks/testableDatesYearOnlyForPlotOnly.txt');
timeWindowLengthIndex = 1;
dateFormat = 'yyyy';
fileID = fopen(filename_str,'w');
fprintf(fileID,'\n');
for i=1:noRolloverTimeWindowsPerLengthtimeWindows(timeWindowLengthIndex)
    fprintf(fileID,'%s ',datestr(testableDatesVector{1,1}{timeIndexMatrix(timeWindowLengthIndex,i)},dateFormat));
    if (i ~= noRolloverTimeWindowsPerLengthtimeWindows(timeWindowLengthIndex))
        fprintf(fileID,'\n');
    end
end
fclose(fileID);

% write tickersVector to file: for plot only!!!
filename_str = sprintf('../data_files/financialNetworks/tickersForPlotOnly.txt');
fileID = fopen(filename_str,'w');
fprintf(fileID,'\n');
for i=1:length(tickersVector{1,1})
    fprintf(fileID,'%s ',tickersVector{1,1}{i});
    if (i ~= length(tickersVector{1,1}))
        fprintf(fileID,'\n');
    end
end
fclose(fileID);

% write testLouvain method temporal communities to file
% filename_str = sprintf('../data_files/financialNetworks/testLouvainMethodTemporalCommunities.dat');
% timeWindowLengthIndex = 1;
% dateFormat = 'yyyy-mm-dd';
% fileID = fopen(filename_str,'w');
% for i=1:n
%     for j=1:noRolloverTimeWindowsPerLengthtimeWindows(timeWindowLengthIndex)
%         fprintf(fileID,'%d ',testLouvainMethodTemporalCommunities(n-i+1,j));
%     end
%     if (i ~= n)
%         fprintf(fileID,'\n');
%     end
% end
% fclose(fileID);


