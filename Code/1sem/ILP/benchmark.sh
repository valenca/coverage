#ILP

j=10

for (( seed=0; seed<10; seed++ )); do
    echo -e "Seed $seed\nPts\tCents\tCoverage\tTime"
    for (( i=2; i<=10; i+=2 )); do
	echo "$j" > tmp
	echo "$i" >> tmp
	python2 ../inputs/uniform.py $seed $j | tail -500 | head -100 >> tmp
	python2 cov_ilp.py bench < tmp
    done
    notify-send "Seed Done" "on to set $(( seed+1 ))"
done

rm tmp


#for (( seed=0; seed<20; seed++ )); do
#    echo -e "Seed $seed\nPts\tCents\tCoverage\tTime"
#    for (( i=40; i<=41; i+=5 )); do
#	echo "$j" > tmp
#	echo "$i" >> tmp
