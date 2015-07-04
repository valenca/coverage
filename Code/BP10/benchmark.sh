#C++

j=30000

for (( i=1000; i<=j; i+=1000 )); do
    for (( seed=0; seed<30; seed++ )); do
	./main < ../benches/clus10_${seed}_${i}.in > tmp2
	cat tmp2 | tail -1 >> res/results_clus10
	./main < ../benches/unif10_${seed}_${i}.in > tmp2
	cat tmp2 | tail -1 >> res/results_unif10
    done
    echo -e "" >> res/results_clus10
    echo -e "" >> res/results_unif15
    notify-send "Batch Done" "size $(( i ))"
done

rm tmp2

j=30000

for (( i=1000; i<=j; i+=1000 )); do
    for (( seed=0; seed<30; seed++ )); do
	./main < ../benches/unif15_${seed}_${i}.in > tmp2
	cat tmp2 | tail -1 >> res/results_unif15
    done
    echo -e "" >> res/results_clus15
    echo -e "" >> res/results_unif15
    notify-send "Batch Done" "size $(( i ))"
done

rm tmp2

j=30000

for (( i=1000; i<=j; i+=1000 )); do
    for (( seed=0; seed<30; seed++ )); do
	./main < ../benches/clus20_${seed}_${i}.in > tmp2
	cat tmp2 | tail -1 >> res/results_clus20
	./main < ../benches/unif20_${seed}_${i}.in > tmp2
	cat tmp2 | tail -1 >> res/results_unif20
    done
    echo -e "" >> res/results_clus20
    echo -e "" >> res/results_unif20
    notify-send "Batch Done" "size $(( i ))"
done

rm tmp2
