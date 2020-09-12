for j in  1 ; do
	echo "$j operation\n"
	for k in 1 2 4 8 16; do
		echo "$k insertion / txn\n"
		echo "DHL_CC"
		echo "Txn/sec"
		for i in 1; do
		/home/dicl/icde2020/codes/DHL -n 1000 -w 1050 -o $k -p 3  -R 128 -t $j -j 0  -S 2
		rm -f j.db*
		done;
		echo "DHL_TC"
		echo "Txn/sec"
		for i in 1; do
		/home/dicl/icde2020/codes/DHL_TC -n 1000 -w 1050 -o $k -p 3  -R 128 -t $j -j 0  -S 2
		rm -f j.db*
		done;
		echo "mvbt_opt"
		echo "Txn/sec"
		for i in 1; do
		/home/dicl/icde2020/codes/mvbt_opt -n 1000 -w 1050 -o $k -p 3  -R 128 -t $j -j 0  -S 2
		rm -f j.db*
		done;
		echo "off_opt"
		echo "Txn/sec"
		for i in 1; do
		/home/dicl/icde2020/codes/off_opt -n 1000 -w 1050 -o $k -p 3  -R 128 -t $j -j 0  -S 2
		rm -f j.db*
		done;
		echo "wal_opt"
		echo "Txn/sec"
		for i in 1; do
		/home/dicl/icde2020/codes/wal_opt -n 1000 -w 1050 -o $k -p 3  -R 128 -t $j -j 1  -S 2
		rm -f j.db*
		done;
		echo "mvbt"
		echo "Txn/sec"
		for i in 1; do
		/home/dicl/icde2020/codes/mvbt -n 1000 -w 1050 -o $k -p 3  -R 128 -t $j -j 0  -S 2
		rm -f j.db*
		done;

	done;

done;
