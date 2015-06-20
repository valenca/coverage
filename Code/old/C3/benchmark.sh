#C++

j=30000

#for (( seed=0; seed<30; seed++ )); do
#echo -e "Seed $seed\nPts\tCents\tCoverage\tTime"
#rm results_unif
#rm results_clus

for (( i=1000; i<=j; i+=1000 )); do
    for (( seed=0; seed<30; seed++ )); do
	echo "$i" > utmp
	echo "0.1" >> utmp
	#echo "$j" > ctmp
	#echo "0.$i" >> ctmp
	python2 ../inputs/uniform.py $seed $j >> utmp
	#python2 ../inputs/cluster.py $seed $j >> ctmp
	./main < utmp > tmp2
	cat tmp2 | tail -1 >> res/results_unif10
	#./main < ctmp > tmp2
	#cat tmp2 | tail -1 >> results_clus20k
    done
    echo -e "" >> res/results_unif10
    #echo -e "" >> results_clus20k
    notify-send "Batch Done" "size $(( i ))"
done

rm tmp2
rm utmp
rm ctmp
