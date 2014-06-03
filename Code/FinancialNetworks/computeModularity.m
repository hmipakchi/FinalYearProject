function [ modularity ] = computeModularity( modularityMatrix, partition )
    n = size(modularityMatrix,1);
    cNorm = 0;
    for i=1:n
        for j=1:n
            cNorm = cNorm + abs(modularityMatrix(i,j));
        end
    end
    
    modularity = 0;
    for i=1:n
        for j=1:n
            if (partition(i) == partition(j))
                modularity = modularity + modularityMatrix(i,j);
            end
        end
    end
    modularity = modularity/cNorm;
end