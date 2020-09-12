name1="/cache/test_dir/result_breakdown_$1"
name2="/cache/test_dir/result_oper_ratio_$1"
#name4="/cache/test_dir/result_recovery_$1"
#name3="/cache/icde2020/result_thread_$1"
tmp1="result_breakdown_$1"
tmp2="result_oper_ratio_$1"
#tmp3="result_thread_$1"
adb pull $name1
adb pull $name2
#adb pull $name4
#adb pull $name3
#scp -P2123 $tmp1 $tmp2 osh829@dumbo.unist.ac.kr:~/TEMP
#scp -P 2123 $tmp1 $tmp2 $tmp3 osh829@dumbo.unist.ac.kr:~/TEMP
