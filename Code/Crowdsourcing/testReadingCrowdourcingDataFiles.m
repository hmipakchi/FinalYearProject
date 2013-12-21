clear;

% read the true value for each task (-1 or 1)
fileID = fopen('data_files/crowdsourcing/parsed_data_files/BarzanMozafari_TaskTrueValues.txt','r');
t = fscanf(fileID, '%*d %d', inf);
fclose(fileID);

% read the responses from each worker for each task (-1, 0 or 1)
[m,mTemp] = size(t);

fileID = fopen('data_files/crowdsourcing/parsed_data_files/BarzanMozafari_WorkersTaskResponses.txt','r');
responseMatrix = fscanf(fileID, '%d', [m,inf])';
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