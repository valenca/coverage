param N;
param D;
param C{i in 1..N,j in 1..N}>=0;

var y{j in 1..N},binary;       
var x{i in 1..N,j in 1..N}, binary;  
var K, >=0,integer;

minimize obj: K;

s.t. limK                        : sum{j in 1..N}(y[j]) <= K;

s.t. excl1 {i in 1..N}           : sum{j in 1..N}(x[i,j]) == 1;

s.t. lim2  {i in 1..N,j in 1..N} : x[i,j]<=y[j];

s.t. dist  {i in 1..N,j in 1..N} : x[i,j]<=C[i,j];


solve;

printf {j in 1..N:y[j]==1} "%d ", j-1;
printf "\n";
for {i in 1..N}{for {j in 1..N} printf "%s ", if x[i,j] then "1" else "0"; printf("\n");}

printf "N = %d,\tK = %f,\t",N,K; 
#printf {j in 1..N}"%i ", y[j];
printf "B = %f,\t",D;
end;
