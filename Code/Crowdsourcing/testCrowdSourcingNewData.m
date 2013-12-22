clear;

% alter collective quaity of the crowd, q, using Barzan Mozafari Data

% read the true value for each task (-1 or 1)
fileID = fopen('data_files/crowdsourcing/parsed_data_files/BarzanMozafari_TaskTrueValues.txt','r');
t = fscanf(fileID, '%*d %d', inf);
fclose(fileID);

% read the tasks completed by each worker (1, 2, ..., m)
fileID = fopen('data_files/crowdsourcing/parsed_data_files/BarzanMozafari_WorkersTaskIds.txt','r');
info = fscanf(fileID, '%d %d', 2);
n = info(1);
r = info(2);

tasksAssignedPerWorker = zeros(n,r);

i=0;
j=1;
while ~feof(fileID)
    tline = fgets(fileID);
    while ischar(tline)
        rowValues = sscanf(tline,'%d', inf);
        [noRowValues,noRowValuesTemp] = size(rowValues);
        for r=1:noRowValues
            tasksAssignedPerWorker(i,j) = rowValues(r);
            j=j+1;
        end
        tline = fgets(fileID);
        j=1;
        i=i+1;
    end
end

fclose(fileID);

% read the responses from each worker for each task (-1, 0 or 1)
m = length(t);
m_str = int2str(m);

responseMatrixTransposed = zeros(n,m);

fileID = fopen('data_files/crowdsourcing/parsed_data_files/BarzanMozafari_WorkersTaskResponses.txt','r');
i=1;
while ~feof(fileID)
    tline = fgets(fileID);
    while ischar(tline)
        rowValues = sscanf(tline,'%d', inf);
        noRowValues = length(rowValues);
        for j=1:noRowValues
            responseMatrixTransposed(i,j) = rowValues(j);
        end
        tline = fgets(fileID);
        i=i+1;
    end
end

fclose(fileID);

responseMatrix = responseMatrixTransposed';

% responseMatrix = dlmread('data_files/crowdsourcing/parsed_data_files/BarzanMozafari_WorkersTaskResponses.txt');

% finished reading data !!!

noClasses = 2;

l = 5;
l_str = int2str(l);

noJobsPerWorker = zeros(n,1);

for j=1:n
    cumsum = 0;
    for i=1:m
        if responseMatrix(i,j) ~= 0
            cumsum = cumsum + 1;
        end
    end
    noJobsPerWorker(j) = cumsum;
end

noTrials = 40;
qUpperBound = 0.40;
qIncrement = qUpperBound/noTrials;

errorsMVestimate = zeros(m, noTrials);
errorsSVestimate = zeros(m, noTrials);
errorsITestimate = zeros(m, noTrials);

currentTrialNumber = 1;
for q=0:qIncrement:qUpperBound
    
    display(currentTrialNumber);
    
    p = zeros(m,1);

    % consider the spammer-hammer model
    for j=1:m
        flipResult = rand(1);
        if flipResult <= q
            p(j) = 1;
        else
            p(j) = 0.5;
        end
    end

    for j=1:n
        for i=1:noJobsPerWorker(j)
            taskId = tasksAssignedPerWorker(j,i);
            flipResult = rand(1);
            if flipResult <= p(j)
                responseMatrix(taskId,j) = t(taskId);
            else
                responseMatrix(taskId,j) = -t(taskId);
            end
        end
    end

    % use Majority-Voting (MV) scheme
    tMVEstimate = majorityVotingAlgorithm(responseMatrix);

    % calculate errors for MV scheme
    for i=1:m
        if t(i) == tMVEstimate(i)
            errorsMVestimate(i,currentTrialNumber) = 0;
        else
            errorsMVestimate(i,currentTrialNumber) = 1;
        end
    end
    
    % use Singular Vector (SV) scheme
    tSVEstimate = singularVectorAlgorithm(responseMatrix, 20);
    
    % calculate errors for Singular Vector (SV) scheme
    for i=1:m
        if t(i) == tSVEstimate(i)
            errorsSVestimate(i,currentTrialNumber) = 0;
        else
            errorsSVestimate(i,currentTrialNumber) = 1;
        end
    end
    
    % use Iterative (IT) scheme
%     tITEstimate = iterativeAlgorithm(tasksAssignedPerWorker, responseMatrix, 10);
    tITEstimate = zeros(m,1);

    % calculate errors for IT scheme
    for i=1:m
        if t(i) == tITEstimate(i)
            errorsITestimate(i,currentTrialNumber) = 0;
        else
            errorsITestimate(i,currentTrialNumber) = 1;
        end
    end
    
    currentTrialNumber = currentTrialNumber + 1;
end

subplot(1,1,1);
title_str = sprintf('plot for estimated with m = %s, l = %s',m_str,l_str);
title(title_str,'FontSize',16);
xlabel('Collective quality of the crowd (q)','FontSize',17);
ylabel('Error','FontSize',16);

errorMVData = zeros(noTrials,1);
errorSVData = zeros(noTrials,1);
errorITData = zeros(noTrials,1);

for i=1:noTrials
    hold on;

    errorMV = sum(errorsMVestimate(:,i)) / m;
    errorSV = sum(errorsSVestimate(:,i)) / m;
    errorIT = sum(errorsITestimate(:,i)) / m;
    
    semilogy(i*qUpperBound/noTrials,errorMV,'Marker','o','MarkerEdgeColor','blue');
    
    dumbThreshold = 0.5;
    if errorSV > dumbThreshold
        errorSV = 1 - errorSV;
    end
    if errorIT > dumbThreshold
        errorIT = 1 - errorIT;
    end

    semilogy(i*qUpperBound/noTrials,errorSV,'Marker','o','MarkerEdgeColor','red');
    
    semilogy(i*qUpperBound/noTrials,errorIT,'Marker','o','MarkerEdgeColor','green');

    errorMVData(i) = errorMV;
    errorSVData(i) = errorSV;
    errorITData(i) = errorIT;
end


% write algorithms errors to file
% fileID = fopen('data_files/crowdsourcing/crowdsourcingErrors_NewData.dat','w');
% for j=1:noTrials
%     fprintf(fileID,'%d ',j*qUpperBound/noTrials);
%     for i=1:3
%         if i == 1
%             fprintf(fileID,'%d ',errorMVData(j));
%         elseif i == 2
%             fprintf(fileID,'%d ',errorSVData(j));
%         else
%             fprintf(fileID,'%d ',errorITData(j));
%         end
%     end
%     fprintf(fileID,'\n');
% end
% fclose(fileID);