#C

j=50

for (( seed=0; seed<5; seed++ )); do
    echo -e "Seed $seed\nPts\tCents\tCoverage\tTime"
    for (( i=32; i<=40; i+=2 )); do
	echo "$i" > tmp
	echo "$(( i/2 ))" >> tmp
	python2 ../inputs/uniform.py $seed | tail -500 | head -100 >> tmp
	timeout 30 ./main < tmp > tmp2
	cat tmp2 | tail -1 tmp2
    done
done

rm tmp2
rm tmp
