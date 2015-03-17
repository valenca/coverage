#C++

j=30000

#for (( seed=0; seed<30; seed++ )); do
#echo -e "Seed $seed\nPts\tCents\tCoverage\tTime"
rm results_unif
rm results_clus

for (( i=1000; i<=j; i+=1000 )); do
    for (( seed=0; seed<30; seed++ )); do
	echo "$i" > utmp
	echo "0.30" >> utmp
	echo "$i" > ctmp
	echo "0.30" >> ctmp
	python2 ../inputs/uniform.py $seed $i >> utmp
	python2 ../inputs/cluster.py $seed $i >> ctmp
	./main < utmp > tmp2
	cat tmp2 | tail -1 >> results_unif
	./main < ctmp > tmp2
	cat tmp2 | tail -1 >> results_clus
    done
    echo -e "" >> results_unif
    echo -e "" >> results_clus
    notify-send "Batch Done" "size $(( i ))"
done

rm tmp2
rm utmp
rm ctmp
