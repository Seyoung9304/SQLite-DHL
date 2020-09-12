for app in "kakaotalk"   ; do
sql="/data/osh/${app}-SQL"
echo $app
for exe in "DHL" "off_opt" ; do
	echo $exe
	#for i in 1 2 3 4 5; do
	sh my_setup.sh
	./$exe -t 3 -z 147 -e $sql > result_real_${app}_$exe
	rm j.db*
	#done
done
for exe in "wal_opt" "wal" ; do
	echo $exe
	#for i in 1 2 3 4 5; do
	sh my_setup.sh
	./$exe -j 1 -t 3 -z 147 -c 100 -e $sql > result_real_${app}_$exe
	rm j.db*
	#done
done
for exe in "shadow_x" ; do
	echo $exe
	#for i in 1 2 3 4 5; do
	sh my_setup.sh
	./$exe -j 5 -t 3 -z 147 -e $sql > result_real_${app}_$exe
	rm j.db*
	#done
done
done
sleep 3
for app in "hangout"  ; do #"kakaotalk" "gmail" ; do
sql="/data/osh/${app}-SQL"
echo $app
for exe in "DHL" "off_opt" ; do
	echo $exe
	#for i in 1 2 3 4 5; do
	sh my_setup.sh
	./$exe -t 3 -z 23 -e $sql > result_real_${app}_$exe
	rm j.db*
	#done
done
for exe in "wal_opt" "wal" ; do
	echo $exe
	#for i in 1 2 3 4 5; do
	sh my_setup.sh
	./$exe -j 1 -t 3 -z 23 -c 10 -e $sql > result_real_${app}_$exe
	rm j.db*
	#done
done
for exe in "shadow_x" ; do
	echo $exe
	#for i in 1 2 3 4 5; do
	sh my_setup.sh
	./$exe -j 5 -t 3 -z 23 -e $sql > result_real_${app}_$exe
	rm j.db*
	#done
done
done
sleep 3
for app in  "facebook"  ; do
sql="/data/osh/${app}-SQL"
echo $app
for exe in "DHL" "off_opt" ; do
	echo $exe
	#for i in 1 2 3 4 5; do
	sh my_setup.sh
	./$exe -t 3 -z 220 -e $sql > result_real_${app}_$exe
	rm j.db*
	#done
done
for exe in "wal_opt" "wal" ; do
	echo $exe
	#for i in 1 2 3 4 5; do
	sh my_setup.sh
	./$exe -j 1 -t 3 -z 220 -c 100 -e $sql > result_real_${app}_$exe
	rm j.db*
	#done
done
for exe in "shadow_x" ; do
	echo $exe
	#for i in 1 2 3 4 5; do
	sh my_setup.sh
	./$exe -j 5 -t 3 -z 220 -e $sql > result_real_${app}_$exe
	rm j.db*
	#done
done
done
sleep 3
for app in   "gmail" ; do
sql="/data/osh/${app}-SQL"
echo $app
for exe in "DHL" "off_opt" ; do
	echo $exe
	#for i in 1 2 3 4 5; do
	sh my_setup.sh
	./$exe -t 3 -z 84 -e $sql > result_real_${app}_$exe
	rm j.db*
	#done
done
for exe in "wal_opt" "wal" ; do
	echo $exe
	#for i in 1 2 3 4 5; do
	sh my_setup.sh
	./$exe -j 1 -t 3 -z 84 -c 50 -e $sql > result_real_${app}_$exe
	rm j.db*
	#done
done
for exe in "shadow_x" ; do
	echo $exe
	#for i in 1 2 3 4 5; do
	sh my_setup.sh
	./$exe -j 5 -t 3 -z 84 -e $sql > result_real_${app}_$exe
	rm j.db*
	#done
done
done
