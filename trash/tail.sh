rm -f j.db*
t=1000
v=1050
for j in 0; do
	echo "$j type\n"
for k in 2; do
  n=$((t*k))
  w=$((v*k))
echo "$n records\n"
echo "$k operation/txn\n"
	echo "DHL_CC"
	for i in 1; do
	sh my_setup.sh
	./Sev_DHL -n $n -w $w -o $k -p 2  -R 128 -t $j -j 0  -T 0 > tail_dhlcc_${j}.dat
	rm -f j.db*
	done;
#	echo "DHL_TC"
#	for i in 1; do
#	sh my_setup.sh
#	./Sev_DHL_TC -n $n -w $w -o $k -p 2  -R 128 -t $j -j 0  -T 0 > tail_dhltc_${j}.dat
#	rm -f j.db*
#	done;
#	echo "mvbt_opt"
#	for i in 1; do
#	sh my_setup.sh
#	./Sev_mvbt_opt -n $n -w $w -o $k -p 2  -R 128 -t $j -j 0  -T 0 > tail_mvbt_${j}.dat
#	rm -f j.db*
#	done;
	echo "wal"
	for i in 1; do
	sh my_setup.sh
	./Sev_wal_ins -n $n -w $w -o $k -p 2  -R 128 -t $j -j 1  -T 0 > tail_wal_${j}.dat
	rm -f j.db*
	done;
	echo "wal_opt"
	for i in 1; do
	sh my_setup.sh
	./Sev_wal_opt_ins -n $n -w $w -o $k -p 2  -R 128 -t $j -j 1  -T 0 > tail_wal_opt_${j}.dat
	rm -f j.db*
	done;
	echo "SHADOW_X"
	for i in 1; do
	sh my_setup.sh
	./Sev_shadow_x_ins -n $n -w $w -o $k -p 2  -R 128 -t $j -j 5  -T 0 > tail_shadow_x_${j}.dat
	rm -f j.db*
	done;

done;
done;
for j in 1 2; do
	echo "$j type\n"
for k in 2; do
  n=$((t*k))
  w=$((v*k))
echo "$n records\n"
echo "$k operation/txn\n"
	echo "DHL_CC"
	for i in 1; do
	sh my_setup.sh
	./Sev_DHL -n $n -w $w -o $k -p 2  -R 128 -t $j -j 0  -T 0 > tail_dhlcc_${j}.dat
	rm -f j.db*
	done;
#	echo "DHL_TC"
#	for i in 1; do
#	sh my_setup.sh
#	./Sev_DHL_TC -n $n -w $w -o $k -p 2  -R 128 -t $j -j 0  -T 0 > tail_dhltc_${j}.dat
#	rm -f j.db*
#	done;
#	echo "mvbt_opt"
#	for i in 1; do
#	sh my_setup.sh
#	./Sev_mvbt_opt -n $n -w $w -o $k -p 2  -R 128 -t $j -j 0  -T 0 > tail_mvbt_${j}.dat
#	rm -f j.db*
#	done;
	echo "wal"
	for i in 1; do
	sh my_setup.sh
	./Sev_wal -n $n -w $w -o $k -p 2  -R 128 -t $j -j 1  -T 0 > tail_wal_${j}.dat
	rm -f j.db*
	done;
	echo "wal_opt"
	for i in 1; do
	sh my_setup.sh
	./Sev_wal_opt -n $n -w $w -o $k -p 2  -R 128 -t $j -j 1  -T 0 > tail_wal_opt_${j}.dat
	rm -f j.db*
	done;
	echo "SHADOW_X"
	for i in 1; do
	sh my_setup.sh
	./Sev_shadow_x -n $n -w $w -o $k -p 2  -R 128 -t $j -j 5  -T 0 > tail_shadow_x_${j}.dat
	rm -f j.db*
	done;

done;
done;
