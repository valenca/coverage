#C++

j=40

for (( seed=0; seed<30; seed++ )); do
    echo -e "Seed $seed\nPts\tCents\tCoverage\tTime"
    for (( i=31; i<$j; i++ )); do
	echo "$j" > tmp
	echo "$i" >> tmp
	python2 ../inputs/uniform.py $seed | tail -500 | head -100 >> tmp
	./main < tmp > tmp2
	cat tmp2 | tail -1 tmp2
    done
done

rm tmp2
rm tmp
