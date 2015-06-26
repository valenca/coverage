#C++

j=30000

for (( i=1000; i<=j; i+=1000 )); do
    for (( seed=0; seed<30; seed++ )); do
	echo "$i" > utmp
	echo "0.10" >> utmp
	python2 ../inputs/cluster.py $seed $i >> utmp
	./main < utmp > tmp2
	cat tmp2 | tail -1 >> res/results_clus10
    done
    echo -e "" >> res/results_clus10
    notify-send "Batch Done" "size $(( i ))"
done

rm tmp2
rm utmp


j=30000

for (( i=1000; i<=j; i+=1000 )); do
    for (( seed=0; seed<30; seed++ )); do
	echo "$i" > utmp
	echo "0.15" >> utmp
	python2 ../inputs/cluster.py $seed $i >> utmp
	./main < utmp > tmp2
	cat tmp2 | tail -1 >> res/results_clus15
    done
    echo -e "" >> res/results_clus15
    notify-send "Batch Done" "size $(( i ))"
done

rm tmp2
rm utmp


j=30000

for (( i=1000; i<=j; i+=1000 )); do
    for (( seed=0; seed<30; seed++ )); do
	echo "$i" > utmp
	echo "0.2" >> utmp
	python2 ../inputs/cluster.py $seed $i >> utmp
	./main < utmp > tmp2
	cat tmp2 | tail -1 >> res/results_clus20
    done
    echo -e "" >> res/results_clus20
    notify-send "Batch Done" "size $(( i ))"
done

rm tmp2
rm utmp
