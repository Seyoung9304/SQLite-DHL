for j in 0 1 2; do
	echo "$j operation\n"
	for k in 1 2 4 8 16; do
		echo "$k insertion / txn\n"
		echo "DHL_CC"
		echo "Txn/sec"
		for i in 1 2 3 4 5 ; do
		sh my_setup.sh
		./DHL -n 1000 -w 1050 -o $k -p 3  -R 128 -t $j -j 0  -T 0
		rm -f j.db*
		done;
		echo "DHL_TC"
		echo "Txn/sec"
		for i in 1 2 3 4 5 ; do
		sh my_setup.sh
		./DHL_TC -n 1000 -w 1050 -o $k -p 3  -R 128 -t $j -j 0  -T 0
		rm -f j.db*
		done;
		echo "mvbt_opt"
		echo "Txn/sec"
		for i in 1 2 3 4 5 ; do
		sh my_setup.sh
		./mvbt_opt -n 1000 -w 1050 -o $k -p 3  -R 128 -t $j -j 0  -T 0
		rm -f j.db*
		done;
		echo "off_opt"
		echo "Txn/sec"
		for i in 1 2 3 4 5 ; do
		sh my_setup.sh
		./off_opt -n 1000 -w 1050 -o $k -p 3  -R 128 -t $j -j 0  -T 0
		rm -f j.db*
		done;
		echo "wal_opt"
		echo "Txn/sec"
		for i in 1 2 3 4 5 ; do
		sh my_setup.sh
		./wal_opt -n 1000 -w 1050 -o $k -p 3  -R 128 -t $j -j 1  -T 0
		rm -f j.db*
		done;
		echo "shadow"
		echo "Txn/sec"
		for i in 1 2 3 4 5 ; do
		sh my_setup.sh
		./shadow -n 1000 -w 1050 -o $k -p 3  -R 128 -t $j -j 5  -T 0
		rm -f j.db*
		done;
		echo "shadow_x"
		echo "Txn/sec"
		for i in 1 2 3 4 5 ; do
		sh my_setup.sh
		./shadow_x -n 1000 -w 1050 -o $k -p 3  -R 128 -t $j -j 5  -T 0
		rm -f j.db*
		done;
#		echo "mvbt"
#		echo "Txn/sec"
#		for i in 1 2 3 4 5 ; do
#		sh my_setup.sh
#		./mvbt -n 1000 -w 1050 -o $k -p 3  -R 128 -t $j -j 0  -T 0
#		rm -f j.db*
#		done;
#		echo "OFF"
#		echo "Txn/sec"
#		for i in 1 2 3 4 5 ; do
#		sh my_setup.sh
#		./original -n 1000 -w 1050 -o $k -p 3  -R 128 -t $j -j 0  -T 0
#		rm -f j.db*
#		done;
#		echo "WAL"
#		echo "Txn/sec"
#		for i in 1 2 3 4 5 ; do
#		sh my_setup.sh
#		./original -n 1000 -w 1050 -o $k -p 3  -R 128 -t $j -j 1  -T 0
#		rm -f j.db*
#		done;

	done;

done;
