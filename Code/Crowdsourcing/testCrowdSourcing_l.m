clear;

% alter the number of queries per task, l

noClasses = 2;

m = 1000;
m_str = int2str(m);

q = 0.15;
q_str = num2str(q);

t = zeros(m,1);
for i=1:m
    t(i) = (2*mod((i-1), noClasses)) - 1;
end

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

noTrials = 30;

errorsMVestimate = zeros(m, noTrials);
errorsSVestimate = zeros(m, noTrials);
errorsITestimate = zeros(m, noTrials);

for l=1:noTrials
    
    display(l);
    
    r = l;
    n = m*l/r;
    responseMatrix = zeros(m,n);
    noJobsPerWorker = zeros(n,1);
    tasksAssignedPerWorker = zeros(n,r);

    for i=1:m
        for lIt=1:l
            selectionFound = 0;
            while selectionFound == 0
                pickLink = ceil(n*r*rand(1));
                pickWorker = mod(pickLink,n) + 1;
                if noJobsPerWorker(pickWorker) < r
                    noJobsPerWorker(pickWorker) = noJobsPerWorker(pickWorker) + 1;
                    tasksAssignedPerWorker(pickWorker,noJobsPerWorker(pickWorker)) = i;
                    selectionFound = 1;
                else
                end
            end
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
            errorsMVestimate(i,l) = 0;
        else
            errorsMVestimate(i,l) = 1;
        end
    end
    
    % use Singular Vector (SV) scheme
    tSVEstimate = singularVectorAlgorithm(responseMatrix, 10);
    
    % calculate errors for Singular Vector (SV) scheme
    for i=1:m
        if t(i) == tSVEstimate(i)
            errorsSVestimate(i,l) = 0;
        else
            errorsSVestimate(i,l) = 1;
        end
    end
    
    % use Iterative (IT) scheme
    tITEstimate = iterativeAlgorithm(tasksAssignedPerWorker, responseMatrix, 10);

    % calculate errors for IT scheme
    for i=1:m
        if t(i) == tITEstimate(i)
            errorsITestimate(i,l) = 0;
        else
            errorsITestimate(i,l) = 1;
        end
    end
end

subplot(1,1,1);
title_str = sprintf('plot for estimates with m = %s, q = %s',m_str,q_str);
title(title_str,'FontSize',16);
xlabel('Numer of queries per task (l)','FontSize',17);
ylabel('Error','FontSize',16);

errorMVData = zeros(noTrials,1);
errorSVData = zeros(noTrials,1);
errorITData = zeros(noTrials,1);

for l=1:noTrials
    hold on;

    errorMV = sum(errorsMVestimate(:,l)) / m;
    errorSV = sum(errorsSVestimate(:,l)) / m;
    errorIT = sum(errorsITestimate(:,l)) / m;

    semilogy(l,errorMV,'Marker','o','MarkerEdgeColor','blue');

    dumbThreshold = 0.5;
    if errorSV > dumbThreshold
        errorSV = 1 - errorSV;
    end
    if errorIT > dumbThreshold
        errorIT = 1 - errorIT;
    end

    semilogy(l,errorSV,'Marker','o','MarkerEdgeColor','red');
    
    semilogy(l,errorIT,'Marker','o','MarkerEdgeColor','green');

    errorMVData(l) = errorMV;
    errorSVData(l) = errorSV;
    errorITData(l) = errorIT;
end

% write algorithms errors to file
fileID = fopen('data_files/crowdsourcing/crowdsourcingErrors_l.dat','w');
for j=1:noTrials
    fprintf(fileID,'%d ',j);
    for i=1:3
        if i == 1
            fprintf(fileID,'%d ',errorMVData(j));
        elseif i == 2
            fprintf(fileID,'%d ',errorSVData(j));
        else
            fprintf(fileID,'%d ',errorITData(j));
        end
    end
    fprintf(fileID,'\n');
end
fclose(fileID);