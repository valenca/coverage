#C++

j=20000

for (( seed=0; seed<1; seed++ )); do
    echo -e "Seed $seed\nPts\tCents\tCoverage\tTime"
    for (( i=j; i<=j; i+=1000 )); do
	echo "$i" > tmp
	echo "0.1" >> tmp
	python2 ../inputs/uniform.py $seed $j >> tmp
	timeout 1800 python2 wrapper.py tmp > tmp2
	cat tmp2 | tail -1 
    done
    notify-send "Seed Done" "on to set $(( seed+1 ))"
done

rm tmp2
rm tmp
