#sync
#echo 3 > /proc/sys/vm/drop_caches
#echo 1 >/sys/devices/system/cpu/cpu0/online
#echo 1 >/sys/devices/system/cpu/cpu1/online
#echo 1 >/sys/devices/system/cpu/cpu2/online
#echo 1 >/sys/devices/system/cpu/cpu3/online
#echo 1 >/sys/devices/system/cpu/cpu4/online
echo performance > /sys/devices/system/cpu/cpu0/cpufreq/scaling_governor
echo performance > /sys/devices/system/cpu/cpu1/cpufreq/scaling_governor
echo performance > /sys/devices/system/cpu/cpu2/cpufreq/scaling_governor
echo performance > /sys/devices/system/cpu/cpu3/cpufreq/scaling_governor
echo performance > /sys/devices/system/cpu/cpu4/cpufreq/scaling_governor
V="0"
while [ "$V" != "1586000" ]
do
	echo 1586000 > /sys/devices/system/cpu/cpu0/cpufreq/scaling_min_freq
	V=`cat /sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_cur_freq`
done
V="0"
while [ "$V" != "1586000" ]
do
	echo 1586000 > /sys/devices/system/cpu/cpu1/cpufreq/scaling_min_freq
	V=`cat /sys/devices/system/cpu/cpu1/cpufreq/cpuinfo_cur_freq`
done
V="0"
while [ "$V" != "1586000" ]
do
	echo 1586000 > /sys/devices/system/cpu/cpu2/cpufreq/scaling_min_freq
	V=`cat /sys/devices/system/cpu/cpu2/cpufreq/cpuinfo_cur_freq`
done
V="0"
while [ "$V" != "1586000" ]
do
	echo 1586000 > /sys/devices/system/cpu/cpu3/cpufreq/scaling_min_freq
	V=`cat /sys/devices/system/cpu/cpu3/cpufreq/cpuinfo_cur_freq`
done
V="0"
while [ "$V" != "520000" ]
do
	echo 520000 > /sys/devices/system/cpu/cpu4/cpufreq/scaling_min_freq
	echo 520000 > /sys/devices/system/cpu/cpu4/cpufreq/scaling_max_freq
	V=`cat /sys/devices/system/cpu/cpu4/cpufreq/cpuinfo_cur_freq`
done
#while [ "$V" != "2600000" ]
#do
#	echo 2600000 > /sys/devices/system/cpu/cpu4/cpufreq/scaling_min_freq
#	echo 2600000 > /sys/devices/system/cpu/cpu4/cpufreq/scaling_max_freq
#	V=`cat /sys/devices/system/cpu/cpu4/cpufreq/cpuinfo_cur_freq`
#done
