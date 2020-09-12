rm -f j.db*
for j in 0 1 2; do # 4 is insert
	echo "$j type\n"
for k in 1; do
echo "$k operation/txn\n"
	echo "DHL_CC"
	echo "fsync write computation"
	for i in 1 2 3 4 5 ; do
	sh my_setup.sh
	./DHL -n 1000 -w 1050 -o $k -p 2  -R 128 -t $j -j 0  -T 0
	rm -f j.db*
	done;
	echo "DHL_TC"
	echo "fsync write computation"
	for i in 1 2 3 4 5 ; do
	sh my_setup.sh
	./DHL_TC -n 1000 -w 1050 -o $k -p 2  -R 128 -t $j -j 0  -T 0
	rm -f j.db*
	done;
	echo "mvbt_opt"
	echo "fsync write computation"
	for i in 1 2 3 4 5 ; do
	sh my_setup.sh
	./mvbt_opt -n 1000 -w 1050 -o $k -p 2  -R 128 -t $j -j 0  -T 0
	rm -f j.db*
	done;
	echo "off_opt"
	echo "fsync write computation"
	for i in 1 2 3 4 5 ; do
	sh my_setup.sh
	./off_opt -n 1000 -w 1050 -o $k -p 2  -R 128 -t $j -j 0  -T 0
	rm -f j.db*
	done;
	echo "wal_opt"
	echo "fsync write computation"
	for i in 1 2 3 4 5 ; do
	sh my_setup.sh
	./wal_opt -n 1000 -w 1050 -o $k -p 2  -R 128 -t $j -j 1  -T 0
	rm -f j.db*
	done;
	echo "shadow"
	echo "fsync write computation"
	for i in 1 2 3 4 5 ; do
	sh my_setup.sh
	./shadow -n 1000 -w 1050 -o $k -p 2  -R 128 -t $j -j 5  -T 0
	rm -f j.db*
	done;
	echo "shadow_x"
	echo "fsync write computation"
	for i in 1 2 3 4 5 ; do
	sh my_setup.sh
	./shadow_x -n 1000 -w 1050 -o $k -p 2  -R 128 -t $j -j 5  -T 0
	rm -f j.db*
	done;
#	echo "mvbt"
#	echo "fsync write computation"
#	for i in 1 2 3 4 5 ; do
#	sh my_setup.sh
#	./mvbt -n 1000 -w 1050 -o $k -p 2  -R 128 -t $j -j 0  -T 0
#	rm -f j.db*
#	done;
#	echo "OFF"
#	echo "fsync write computation"
#	for i in 1 2 3 4 5 ; do
#	sh my_setup.sh
#	./original -n 1000 -w 1050 -o $k -p 2  -R 128 -t $j -j 0  -T 0
#	rm -f j.db*
#	done;
#	echo "WAL"
#	echo "fsync write computation"
#	for i in 1 2 3 4 5 ; do
#	sh my_setup.sh
#	./original -n 1000 -w 1050 -o $k -p 2  -R 128 -t $j -j 1  -T 0
#	rm -f j.db*
#	done;

#done;
done;
done;
