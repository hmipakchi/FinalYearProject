function [ indicators, mu ] = testKMeansClustering( data, noClusters )
%testKMeansClustering: implements K-means clustering algorithm
%   Inputs: data in form of matrix: each row is a data point, each column 
%           represents an element of a particular data point (data), number of
%           clusters to identify (noClusters)
%   Outputs: matrix of indicator variables denoting which cluster each data
%            point belongs to (indicators) , centre's of each cluster (mu)

    [numberDataPoints, dimensionData] = size(data);
    indicators = zeros(numberDataPoints, noClusters);
    mu = zeros(noClusters, dimensionData);
    
    % initialise values for mu
    if numberDataPoints >= noClusters
        for i=1:noClusters
            mu(i,:) = data(i,:);
        end
    else
        for i=1:numberDataPoints
            mu(i,:) = data(i,:);
        end
    end
    
    % define variables for objective function value
    convergenceBound = 0.000001;
    objectiveFunctionValueImprovement = Inf;
    objectiveFunctionValue = Inf;
    
    % run iterative K-means custering
    noIterations = 0;
    while objectiveFunctionValueImprovement > convergenceBound
        
        % minimise objective with respect to indicator variable, keeping
        % means fixed
        for n=1:numberDataPoints
            argmin = 0;
            min = Inf;
            for k=1:noClusters
                distanceBetweenDataPointAndClusterCentre = data(n,:)-mu(k,:);
                if (norm(distanceBetweenDataPointAndClusterCentre)^2) < min
                    argmin = k;
                    min = norm(distanceBetweenDataPointAndClusterCentre)^2;
                end
            end
            % update indicator variable value for this particular data
            % point
            for j=1:noClusters
                if j == argmin
                    indicators(n,j) = 1;
                else
                    indicators(n,j) = 0;
                end
            end
        end
            
        % minimise objective with respect to means, keeping
        % indicator variables fixed
        for k=1:noClusters
            sumDataPointsBelongToCluster = zeros(1,dimensionData);
            numberDataPointsBelongToCluster = 0;
            for n=1:numberDataPoints
                numberDataPointsBelongToCluster = numberDataPointsBelongToCluster + indicators(n,k);
                sumDataPointsBelongToCluster = sumDataPointsBelongToCluster + (indicators(n,k) * data(n,:));
            end
            mu(k,:) = sumDataPointsBelongToCluster ./ numberDataPointsBelongToCluster;
        end
        
        % check improvement in objective function value
        previousObjectiveFunctionValue = objectiveFunctionValue;
        objectiveFunctionValue = 0;
        for n=1:numberDataPoints
            for k=1:noClusters
                distanceBetweenDataPointAndClusterCentre = data(n,:)-mu(k,:);
                objectiveFunctionValue = objectiveFunctionValue + (indicators(n,k) * (norm(distanceBetweenDataPointAndClusterCentre)^2));
            end
        end
        objectiveFunctionValueImprovement = previousObjectiveFunctionValue - objectiveFunctionValue;
        
        noIterations = noIterations + 1;
    end
    
    display(objectiveFunctionValue);
end

