rm -f j.db*
for k in 1 2 4 8 16; do
echo "$k operation/txn\n"
	echo "DHL_CC"
	for i in 1 2 3 4 5 6 7 8 9 10; do
	sh my_setup.sh
	./DHL_recovery -n 1200 -o $k -R 128 -j 0 -r 0
	sh my_setup.sh
	./DHL_recovery -n 1200 -o $k -R 128 -j 0 -r 1
	rm -f oh.db*
	done;
	echo "mvbt"
	for i in 1 2 3 4 5 6 7 8 9 10; do
	sh my_setup.sh
	./mvbt_recovery -n 1200 -o $k -R 128 -j 0 -r 0
	sh my_setup.sh
	./mvbt_recovery -n 1200 -o $k -R 128 -j 0 -r 1
	rm -f oh.db*
	done;
	echo "WAL"
	for i in 1 2 3 4 5 6 7 8 9 10; do
	sh my_setup.sh
	./original_recovery -n 1200 -o $k -R 128 -j 1 -r 0
	sh my_setup.sh
	./original_recovery -n 1200 -o $k -R 128 -j 1 -r 1
	rm -f oh.db*
	done;
	echo "SHADOW"
	for i in 1 2 3 4 5 6 7 8 9 10; do
	sh my_setup.sh
	./shadow_recovery -n 1200 -o $k -R 128 -j 5 -r 0
	sh my_setup.sh
	./shadow_recovery -n 1200 -o $k -R 128 -j 5 -r 1
	rm -f oh.db*
	done;

#done;
done;
