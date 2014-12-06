param N;
param K;
param C{i in 1..N,j in 1..N}>=0;

var y{1..N},binary;       
var x{i in 1..N,j in 1..N}, binary;  
var D, >=0;

minimize obj: D;

s.t. dist1 {i in 1..N-1,j in i+1..N}: C[i,j]*x[i,j]<=D;
s.t. dist2 {i in 1..N-1,j in i+1..N}: C[j,i]*x[j,i]<=D;

s.t. limK : sum{i in 1..N}(y[i])==K;

s.t. excl1 {j in 1..N}: sum{i in 1..N}(x[i,j]) >= 1;
s.t. lim2  {i in 1..N}: sum{j in 1..N}(x[i,j]) <= N*y[i];




solve;

printf {i in 1..N:y[i]==1} "%i ", i-1;
printf "\n";

for {i in 1..N}{for {j in 1..N} printf "%s ", if x[i,j] then "1" else "0"; printf("\n");}
printf "%f\n",D;
end;
