#ILP

j=50

for (( seed=0; seed<10; seed++ )); do
    echo -e "Seed $seed\nPts\tCents\tCoverage\tTime"
    for (( i=32; i<=40; i+=2 )); do
	echo "$i" > tmp
	echo "$(( i/2 ))" >> tmp
	python2 ../inputs/uniform.py $seed | tail -500 | head -100 >> tmp
	python2 cov_ilp.py bench < tmp
    done
done

rm tmp


#for (( seed=0; seed<20; seed++ )); do
#    echo -e "Seed $seed\nPts\tCents\tCoverage\tTime"
#    for (( i=40; i<=41; i+=5 )); do
#	echo "$j" > tmp
#	echo "$i" >> tmp
