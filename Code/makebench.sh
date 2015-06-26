#C++

j=30000

for (( i=1000; i<=j; i+=1000 )); do
    for (( seed=0; seed<30; seed++ )); do
	echo "$i" >  benches/clus10_${seed}_${i}.in
	echo "0.10" >>  benches/clus10_${seed}_${i}.in
	python2 inputs/cluster.py $seed $i >> benches/clus10_${seed}_${i}.in
	#./main <  benches/clus10_${seed}_${i}.in > tmp2
	#cat tmp2 | tail -1 >> res/results_clus10
    done
    #echo -e "" >> res/results_clus10
    notify-send "Batch Done" "size $(( i ))"
done

j=30000

for (( i=1000; i<=j; i+=1000 )); do
    for (( seed=0; seed<30; seed++ )); do
	echo "$i" > benches/clus15_${seed}_${i}.in
	echo "0.15" >> benches/clus15_${seed}_${i}.in
	python2 inputs/cluster.py $seed $i >> benches/clus15_${seed}_${i}.in
	#./main < benches/clus15_${seed}_${i}.in > tmp2
	#cat tmp2 | tail -1 >> res/results_clus15
    done
    #echo -e "" >> res/results_clus15
    notify-send "Batch Done" "size $(( i ))"
done

j=30000

for (( i=1000; i<=j; i+=1000 )); do
    for (( seed=0; seed<30; seed++ )); do
	echo "$i" > benches/clus20_${seed}_${i}.in
	echo "0.2" >> benches/clus20_${seed}_${i}.in
	python2 inputs/cluster.py $seed $i >> benches/clus20_${seed}_${i}.in
	#./main < benches/clus20_${seed}_${i}.in > tmp2
	#cat tmp2 | tail -1 >> res/results_clus20
    done
    #echo -e "" >> res/results_clus20
    notify-send "Batch Done" "size $(( i ))"
done

