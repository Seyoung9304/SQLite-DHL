rm -f j.db*
rm $1
touch $1
for j in 4; do # 4 is insert
	echo "$j type\n" 
	echo "$j type\n" >> $1
for k in 1; do
echo "$k operation/txn\n" 
echo "$k operation/txn\n" >> $1
#	echo "OFF_OPT" 
#	echo "OFF_OPT" >> $1
#	echo "fsync write computation" >> $1
#	for i in 1 2 3 4 5; do
#	sh my_setup.sh
#	./off_opt_ins -n 1000 -w 1050 -o $k -p 1  -R 128 -t $j -j 0  >> $1
#	rm -f j.db*
#	done;
#	echo "DHL_CC" 
#	echo "DHL_CC" >> $1
#	echo "fsync write computation" >> $1
#	for i in 1 2 3 4 5; do
#	sh my_setup.sh
#	./DHL -n 1000 -w 1050 -o $k -p 1  -R 128 -t $j -j 0  >> $1
#	rm -f j.db*
#	done;
#	echo "DHL_TC" 
#	echo "DHL_TC" >> $1
#	echo "fsync write computation" >> $1
#	for i in 1 2 3 4 5; do
#	sh my_setup.sh
#	./DHL_TC -n 1000 -w 1050 -o $k -p 1  -R 128 -t $j -j 0  >> $1
#	rm -f j.db*
#	done;
#	echo "mvbt_opt" 
#	echo "mvbt_opt" >> $1
#	echo "fsync write computation" >> $1
#	for i in 1 2 3 4 5; do
#	sh my_setup.sh
#	./mvbt_opt -n 1000 -w 1050 -o $k -p 1  -R 128 -t $j -j 0  >> $1
#	rm -f j.db*
#	done;
#	echo "WAL_OPT" 
#	echo "WAL_OPT" >> $1
#	echo "fsync write computation" >> $1
#	for i in 1 2 3 4 5; do
#	sh my_setup.sh
#	./wal_opt_ins -n 1000 -w 1050 -o $k -p 1  -R 128 -t $j -j 1  >> $1
#	rm -f j.db*
#	done;
#	echo "OFF" 
#	echo "OFF" >> $1
#	echo "fsync write computation" >> $1
#	for i in 1 2 3 4 5; do
#	sh my_setup.sh
#	./original -n 1000 -w 1050 -o $k -p 1  -R 128 -t $j -j 0  >> $1
#	rm -f j.db*
#	done;
#	echo "mvbt" 
#	echo "mvbt" >> $1
#	echo "fsync write computation" >> $1
#	for i in 1 2 3 4 5; do
#	sh my_setup.sh
#	./mvbt -n 1000 -w 1050 -o $k -p 1  -R 128 -t $j -j 0  >> $1
#	rm -f j.db*
#	done;
	echo "SHADOW_X" 
	echo "fsync write computation" >> $1
	echo "SHADOW_X" >> $1
	for i in 1 2 3 4 5; do
	sh my_setup.sh
	./shadow_x_ins -n 1000 -w 1050 -o $k -p 1  -R 128 -t $j -j 5  >> $1
	rm -f j.db*
	done;
#	echo "WAL" 
#	echo "WAL" >> $1
#	echo "fsync write computation" >> $1
#	for i in 1 2 3 4 5; do
#	sh my_setup.sh
#	./wal_ins -n 1000 -w 1050 -o $k -p 1  -R 128 -t $j -j 1  >> $1
#	rm -f j.db*
#	done;
done;
done;
