function [e] = get_community_matrix_modified(correlationMatrix,com)

    % Number of communities
    nc = length(unique(com));

    % Initialise adjacency matrix list
    e = zeros(nc,nc);

    for i=1:length(correlationMatrix)
        for j=i:length(correlationMatrix)
            ic = com(i);
            jc = com(j);
            e(ic,jc) = e(ic,jc) + correlationMatrix(i,j);
        end
    end
    
end

