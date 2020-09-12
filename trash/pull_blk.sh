for i in "DHLCC" "DHLTC" "WAL" "OFF" "MVBT" "SHADOW" "SHADOW_X"; do 
name1="/cache/test_dir/blktrace/result_${i}.po"
name2="/cache/test_dir/blktrace/result_${i}_IO.po"
tmp1="result_2_${i}.po"
adb pull $name1
adb pull $name2
done;
