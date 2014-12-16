param N;
param K;
param L;
param D{l in 1..L} >=0;
param C{i in 1..N,j in 1..N}>=0;

var y{j in 1..N},binary;
var z{l in 1..L},binary;

minimize obj: D[1] + sum{l in 2..L}(D[l]-D[l-1])*z[l];


s.t. c1                         : sum{j in 1..N}(y[j]) >= 1;

s.t. c2                         : sum{j in 1..N}(y[j]) <= K;

s.t. c3	{i in 1..N,l in 1..L}   : z[l] + sum{j in 1..N: C[i,j] < D[l]} (y[j])>=1;


solve;

printf {j in 1..N:y[j]==1} "%i ", j-1;
printf "\n";

#for {i in 1..N}{for {j in 1..N} printf "%s ", if x[i,j] then "1" else "0"; printf("\n");}
printf "%f\n",D[1] + sum{l in 2..L}(D[l]-D[l-1])*z[l];
end;
