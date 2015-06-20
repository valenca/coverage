#C++

j=30000

for (( i=30000; i<=j; i+=1000 )); do
    for (( seed=0; seed<30; seed++ )); do
	echo "$i" > utmp
	echo "0.15" >> utmp
	python2 ../inputs/uniform.py $seed $j >> utmp
	./main < utmp > tmp2
	cat tmp2 | tail -1 >> res/results_unif15
    done
    echo -e "" >> res/results_unif15
    notify-send "Batch Done" "size $(( i ))"
done

rm tmp2
rm utmp
