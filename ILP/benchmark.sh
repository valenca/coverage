#ILP

j=40


for (( seed=0; seed<30; seed++ )); do
    echo -e "Seed $seed\nPts\tCents\tCoverage\tTime"
    for (( i=31; i<$j; i++ )); do
	echo "$j" > tmp
	echo "$i" >> tmp
	python2 ../inputs/uniform.py $seed | tail -500 | head -100 >> tmp
	python2 cov_ilp.py bench < tmp
    done
done

rm tmp
