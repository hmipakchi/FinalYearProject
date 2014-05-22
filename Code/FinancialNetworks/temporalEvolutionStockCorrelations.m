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
% rolloverPeriod = 5; % uncomment for testing
rolloverPeriod = 30;
rolloverPeriod_str = num2str(rolloverPeriod);

timeIndexMatrix = zeros(nolengthTimeWindows,floor((T-min(lengthTimeWindowVector))/rolloverPeriod));

meanCorrelationCoefficientsMatrix = -100.*ones(nolengthTimeWindows,floor((T-min(lengthTimeWindowVector))/rolloverPeriod));
varianceCorrelationCoefficientsMatrix = -100.*ones(nolengthTimeWindows,floor((T-min(lengthTimeWindowVector))/rolloverPeriod));
skewnessCorrelationCoefficientsMatrix = -100.*ones(nolengthTimeWindows,floor((T-min(lengthTimeWindowVector))/rolloverPeriod));

noRolloverTimeWindowsPerLengthtimeWindows = zeros(nolengthTimeWindows,1);

for i=1:nolengthTimeWindows
    
    lengthTimeWindow = lengthTimeWindowVector(i);
    noRolloverTimeWindows = floor((T-lengthTimeWindow)/rolloverPeriod);
    noRolloverTimeWindowsPerLengthtimeWindows(i,1) = noRolloverTimeWindows;
    
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
        
        meanCorrelationCoefficientsMatrix(i,j) = calculateMeanCorrelationValue(sampleCrossCorrelationMatrix);
        varianceCorrelationCoefficientsMatrix(i,j) = calculateVarianceCorrelationValue(sampleCrossCorrelationMatrix);
        skewnessCorrelationCoefficientsMatrix(i,j) = calculateSkewnessCorrelationValue(sampleCrossCorrelationMatrix);
    end
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
