rm -f j.db*
t=20480
for j in 8 16 32 64 128; do
	echo "$j type\n"
for k in 1; do
echo "$k operation/txn\n"
#for l in 1 2 4 8 16; do
#echo "0 threads\n"
  n=$((t/ j))
  echo $n
	echo "DHL_CC"
	for i in 1; do
	sh my_setup.sh
	./DHL -n $n -o $k -p 6  -R $j -t 0 -j 0  -T 0
	rm -f j.db*
	done;
	echo "mvbt"
	for i in 1; do
	sh my_setup.sh
	./mvbt -n $n -o $k -p 6  -R $j -t 0 -j 0  -T 0
	rm -f j.db*
	done;
	echo "OFF"
	for i in 1; do
	sh my_setup.sh
	./original -n $n -o $k -p 6  -R $j -t 0 -j 0  -T 0
	rm -f j.db*
	done;

#done;
done;
done;
