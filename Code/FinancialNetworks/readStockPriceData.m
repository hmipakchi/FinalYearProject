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
    tickersVector{1,1}{rowsToDelete(candidateRow)} = [];
end

[n,T] = size(priceMatrix);

n_str = num2str(n);
T_str = num2str(T);

% calculate logarithmic returns for stock price data
logReturns = calculateLogarithmicReturns(priceMatrix);
% startDate = '2011-01-01';
% endDate = '2013-01-01';
% startDateIndex = getDateIndexFromDate(testableDatesVector,startDate);
% endDateIndex = getDateIndexFromDate(testableDatesVector,endDate);
% logReturns = calculateLogarithmicReturns(priceMatrix(:,startDateIndex:endDateIndex));

% calculate expected returns for stock price data between 2011-2013
% startDate = '2011-01-01';
% endDate = '2013-01-01';
% startDateIndex = getDateIndexFromDate(testableDatesVector,startDate);
% endDateIndex = getDateIndexFromDate(testableDatesVector,endDate);
% returns = calculateReturns(priceMatrix(:,startDateIndex:endDateIndex));
% expectedReturns = calculateTimeAverage(returns);
% varianceReturns = calculateSampleVariance(returns);

% write sample expected returns and volatilities to file
% filename_str = sprintf('../data_files/financialNetworks/expectedReturnsAndVolatility_n_%s_T_%s.dat',n_str,num2str(endDateIndex-startDateIndex+1));
% fileID = fopen(filename_str,'w');
% for i=1:n
%     fprintf(fileID,'%d %d ',sqrt(varianceReturns(i)),expectedReturns(i));
%     fprintf(fileID,'\n');
% end
% fclose(fileID);

% write price and log-return time series for first stock in list
% whichStock = 1;
% dateFormat = 'yyyy-mm-dd';
% filename = sprintf('../data_files/financialNetworks/price_and_logreturn_timeseries_%s.dat',tickersVector{1,1}{whichStock});
% fileID = fopen(filename,'w');
% for i=1:T
%     fprintf(fileID,'%s %d %d',datestr(testableDatesVector{1,1}{i},dateFormat), priceMatrix(whichStock,i),logReturns(whichStock,i));
%     fprintf(fileID,'\n');
% end
% fclose(fileID);

% calculate calculate time average and sample variance of logarithmic returns
timeAverage = calculateTimeAverage(logReturns);
meanSquareTimeAverage = calculateMeanSquareTimeAverage(logReturns);
sampleVariance = calculateSampleVariance(logReturns);

% calculate filtered logarithmic returns
logReturns = calculateFilteredLogarithmicReturns(logReturns);

% calculate sample cross-correlation matrix of filtered logarithmic returns
meanProductTimeAverage = calculateMeanProductTimeAverage(logReturns);
sampleCrossCorrelationMatrix = calculateSampleCrossCorrelationMatrix(logReturns);

% write sample cross-correlation returns to file
% filename_str = sprintf('../data_files/financialNetworks/crossCorrelation_n_%s_T_%s.dat',n_str,T_str);
% fileID = fopen(filename_str,'w');
% for i=1:n
%     for j=1:n
%         fprintf(fileID,'%d ',sampleCrossCorrelationMatrix(i,j));
%     end
%     fprintf(fileID,'\n');
% end
% fclose(fileID);

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

% nbins = 200;
% [spectrum,lambda] = hist(eigenvalues,nbins);
% plot(lambda,spectrum/trapz(lambda,spectrum),'red','LineWidth',2);
% hold on;
% plot(theoreticalSpectrum(:,1),theoreticalSpectrum(:,2),'blue','LineWidth',2);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
[V,D] = eig(sampleCrossCorrelationMatrix);
modularityMatrix = zeros(n,n);
for i=1:n
    if (eigenvalues(i) > maxTheoreticalEigenvalue) && (eigenvalues(i) < maxEmpiricalEigenvalue)
        disp(i);
        modularityMatrix = modularityMatrix + (eigenvalues(i).*(V(:,i)*V(:,i)'));
    end
end
[fastNewmanModifiedCommunities,fastNewmanModifiedModularity] = fast_newman_modified(modularityMatrix);

weightedAdjacencyMatrix = zeros(n,n);
for i=1:n
    for j=1:n
        weightedAdjacencyMatrix(i,j) = 0.5*(sampleCrossCorrelationMatrix(i,j)+1) - delta(i,j);
    end
end
[fastNewmanCommunities,fastNewmanModularity] = fast_newman(weightedAdjacencyMatrix);
[montanariCommunities] = montanari_modularity(weightedAdjacencyMatrix);

fastNewmanModifiedMethod_filename_str = '../data_files/financialNetworks/outputFTSE100FastNewmanModifiedMethod.gexf';
fastNewmanMethod_filename_str = '../data_files/financialNetworks/outputFTSE100FastNewmanMethod.gexf';
montanariMethod_filename_str = '../data_files/financialNetworks/outputFTSE100MontanariMethod.gexf';

% writeGroupedCommunitiesByTickersToGexfFile(fastNewmanModifiedMethod_filename_str, fastNewmanModifiedCommunities, tickersVector);
% writeGroupedCommunitiesByTickersToGexfFile(fastNewmanMethod_filename_str, fastNewmanCommunities, tickersVector);
% writeGroupedCommunitiesByTickersToGexfFile(montanariMethod_filename_str, montanariCommunities, tickersVector);

fastNewmanModifiedMethod_Industries_filename_str = '../data_files/financialNetworks/outputFTSE100IndustriesFastNewmanModifiedMethod.gexf';
fastNewmanMethod_Industries_filename_str = '../data_files/financialNetworks/outputFTSE100IndustriesFastNewmanMethod.gexf';
montanariMethod_Industries_filename_str = '../data_files/financialNetworks/outputFTSE100IndustriesMontanariMethod.gexf';

% writeGroupedCommunitiesByIndustriesToGexfFile(fastNewmanModifiedMethod_Industries_filename_str , fastNewmanModifiedCommunities, ICBIndustriesVector);
% writeGroupedCommunitiesByIndustriesToGexfFile(fastNewmanMethod_Industries_filename_str , fastNewmanCommunities, ICBIndustriesVector);
% writeGroupedCommunitiesByIndustriesToGexfFile(montanariMethod_Industries_filename_str , montanariCommunities, ICBIndustriesVector);

fastNewmanModifiedRenormalisedFilteredCorrelation = calculateRenormalisedFilteredCorrelationMatrix(modularityMatrix, fastNewmanModifiedCommunities);
fastNewmanRenormalisedFilteredCorrelation = calculateRenormalisedFilteredCorrelationMatrix(modularityMatrix, fastNewmanCommunities);
montanariRenormalisedFilteredCorrelation = calculateRenormalisedFilteredCorrelationMatrix(modularityMatrix, montanariCommunities);