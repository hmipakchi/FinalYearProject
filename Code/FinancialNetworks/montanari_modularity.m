function [ groups ] = montanari_modularity( A )

n = size(A, 1);
deg = full(sum(A)');
m = sum(deg)/2;
B = full(A) - deg*deg'/(2*m);

%initialization
q = 1;
groups = ones(n, 1); %initialize all to the same cluster
tol = 1e-9;
delQ = 0;
size_tol=2;

cnt = 1;
while(1)
    flag = 0;
    q = cnt;
    changearray = zeros(q, 1);
    for iter=1:q
        Ba = B(groups==iter, groups==iter);
        if(size(Ba, 1) <= size_tol)
            continue
        end
        Ba = Ba - diag(sum(Ba));
        [V, l] = eigs(Ba, 1, 'la');
        V = sign(V);
        delQa = V'*Ba*V/(4*m);
        flag = (flag || (delQa>tol));
        pos = sum(V>0);
        neg = sum(V<0);
        changearray(iter) = delQa;
        if(delQa>tol)
            groups(groups==iter) = (1+V)*(cnt+1)/2 + (1-V)*(iter/2);
            cnt = cnt+1;
        end
    end
    if(flag ==0)
        break
    end
end