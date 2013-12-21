function [ itEstimate ] = iterativeAlgorithm( tasksAssignedPerWorker, responseMatrix, noIterations )
%   iterativeAlgorithm
%   Input: tasksAssignedPerWorker, response matrix, number of iterations to run
%   Output: estimate of tasks using Iterative Algorithm

    [m,n] = size(responseMatrix);
    
    [n,r] = size(tasksAssignedPerWorker);
    
    % construct tasksWorkerGraphAdjacencyMatrix
    tasksWorkerGraphAdjacencyMatrix = zeros(m,n);
    for i=1:r
        for j=1:n
            tasksWorkerGraphAdjacencyMatrix(tasksAssignedPerWorker(j,i),j) = 1;
        end
    end
    
    % find degrees of all task nodes
    tasksDegreeVector = zeros(m,1);
    for i=1:m
        tasksDegreeVector(i) = sum(tasksWorkerGraphAdjacencyMatrix(i,:));
    end
    
    % find degrees of all worker nodes
    workersDegreeVector = zeros(n,1);
    for j=1:n
        workersDegreeVector(j) = sum(tasksWorkerGraphAdjacencyMatrix(:,j));
    end
    
    % find neighbours of all task nodes
    tasksNeighboursMatrix = zeros(m,n);
    for i=1:m
        tasksIterator = 1;
        for j=1:n
            if tasksWorkerGraphAdjacencyMatrix(i,j) == 1
                tasksNeighboursMatrix(i,tasksIterator) = j;
                tasksIterator = tasksIterator + 1;
            end
        end
    end
    
    % find neighbours of all worker nodes
    workersNeighboursMatrix = zeros(n,m);
    for j=1:n
        workersIterator = 1;
        for i=1:m
            if tasksWorkerGraphAdjacencyMatrix(i,j) == 1
                workersNeighboursMatrix(j,workersIterator) = i;
                workersIterator = workersIterator + 1;
            end
        end
    end
    
    itEstimate = zeros(m,1);
    
    y = 1 + randn(m,n);
    x = zeros(m,n);
    
    for k=1:noIterations
        for i=1:m
            for j=1:n
                if tasksWorkerGraphAdjacencyMatrix(i,j) == 1
                    cumsum = 0;
                    for iNeighbour=1:tasksDegreeVector(i)
                        jPrime = tasksNeighboursMatrix(i,iNeighbour);
                        if jPrime ~= j
                            cumsum = cumsum + (responseMatrix(i,jPrime)*y(i,jPrime));
                        end
                    end
                    x(i,j) = cumsum;
                end
            end
        end
        for i=1:m
            for j=1:n
                if tasksWorkerGraphAdjacencyMatrix(i,j) == 1
                    cumsum = 0;
                    for jNeighbour=1:workersDegreeVector(j)
                        iPrime = workersNeighboursMatrix(j,jNeighbour);
                        if iPrime ~= i
                            cumsum = cumsum + (responseMatrix(iPrime,j)*x(iPrime,j));
                        end
                    end
                    y(i,j) = cumsum;
                end
            end
        end
    end
    
    for i=1:m
        cumsum = 0;
        for iNeighbour=1:tasksDegreeVector(i)
            j = tasksNeighboursMatrix(i,iNeighbour);
            cumsum = cumsum + (responseMatrix(i,j)*y(i,j));
        end
        itEstimate(i) = cumsum;
        if itEstimate(i) >= 0
            itEstimate(i) = 1;
        else
            itEstimate(i) = -1;
        end
    end
end
