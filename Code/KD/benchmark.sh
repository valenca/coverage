#C++

j=30000

for (( i=1000; i<=j; i+=1000 )); do
    for (( seed=0; seed<30; seed++ )); do
	timeout 10s ./main < ../benches/clus20_${seed}_${i}.in > tmp2
	cat tmp2 | tail -1 >> res/results_clus20
    done
    echo -e "" >> res/results_clus20
    notify-send "Batch Done" "size $(( i ))"
done

rm tmp2
