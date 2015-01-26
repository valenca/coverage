#C

j=40

for (( seed=5; seed<40; seed++ )); do
    echo -e "Seed $seed\nPts\tCents\tCoverage\tTime"
    for (( i=2; i<=40; i+=2 )); do
	echo "$j" > tmp
	echo "$i" >> tmp
	python2 ../inputs/uniform.py $seed $j | tail -500 | head -100 >> tmp
	timeout 1800 ./main < tmp > tmp2
	cat tmp2 | tail -1 tmp2
    done
    notify-send "Seed Done" "on to set $(( seed+1 ))"
done

rm tmp2
rm tmp
