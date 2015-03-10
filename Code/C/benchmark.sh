#C++

j=1000

for (( seed=1; seed<10; seed++ )); do
    echo -e "Seed $seed\nPts\tCents\tCoverage\tTime"
    for (( i=0; i<j; i+=100 )); do
	echo "$j" > tmp
	echo "$(( i ))" >> tmp
	python2 ../inputs/uniform.py $seed $i | tail -500 | head -100 >> tmp
	timeout 1800 python2 wrapper.py tmp > tmp2
	cat tmp2 | tail -1 tmp2
    done
    notify-send "Seed Done" "on to set $(( seed+1 ))"
done

rm tmp2
rm tmp
