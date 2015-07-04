#C++

j=30000

for (( i=1000; i<=j; i+=1000 )); do
    for (( seed=0; seed<30; seed++ )); do
	echo "$i" >  benches/unif10_${seed}_${i}.in
	echo "0.10" >>  benches/unif10_${seed}_${i}.in
	python2 inputs/uniform.py $seed $i >> benches/unif10_${seed}_${i}.in
	#./main <  benches/unif10_${seed}_${i}.in > tmp2
	#cat tmp2 | tail -1 >> res/results_unif10
    done
    #echo -e "" >> res/results_unif10
    notify-send "Batch Done" "size $(( i ))"
done

j=30000

for (( i=1000; i<=j; i+=1000 )); do
    for (( seed=0; seed<30; seed++ )); do
	echo "$i" > benches/unif15_${seed}_${i}.in
	echo "0.15" >> benches/unif15_${seed}_${i}.in
	python2 inputs/uniform.py $seed $i >> benches/unif15_${seed}_${i}.in
	#./main < benches/unif15_${seed}_${i}.in > tmp2
	#cat tmp2 | tail -1 >> res/results_unif15
    done
    #echo -e "" >> res/results_unif15
    notify-send "Batch Done" "size $(( i ))"
done

j=30000

for (( i=1000; i<=j; i+=1000 )); do
    for (( seed=0; seed<30; seed++ )); do
	echo "$i" > benches/unif20_${seed}_${i}.in
	echo "0.2" >> benches/unif20_${seed}_${i}.in
	python2 inputs/uniform.py $seed $i >> benches/unif20_${seed}_${i}.in
	#./main < benches/unif20_${seed}_${i}.in > tmp2
	#cat tmp2 | tail -1 >> res/results_unif20
    done
    #echo -e "" >> res/results_unif20
    notify-send "Batch Done" "size $(( i ))"
done

