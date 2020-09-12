name1="result_breakdown_$1"
name1_opt="result_breakdown_opt_$1"
name2="result_oper_ratio_$1"
name3="result_tpcc_$1"
name4="result_recovery_$1"
#rm -f Sev_*
#cd /storage/temp_dir
#cp DHL DHL_TC mvbt_opt off_opt off_opt_ins wal_opt shadow shadow_x original mvbt /cache/test_dir/
#cd -
echo "breakdown"
sh breakdown.sh $name1
#echo "breakdown_opt"
#sh breakdown_opt.sh $name1_opt
echo "oper_ratio"
sh oper_ratio.sh $name2

#rm -f DHL DHL_TC mvbt_opt off_opt off_opt_ins wal_opt shadow shadow_x original mvbt

#cd /storage/temp_dir
#cp Sev_* /cache/test_dir/
#cd -
#echo "tail"
#sh tail.sh
#
#rm -f Sev_*
#
#
#echo "tpcc"
#cd tpcc
#sh tpcc.sh $name3
#cd ..

#cd /storage/temp_dir
#cp *_recovery /cache/test_dir
#cd -
#echo "recovery"
#sh recovery.sh > $name4
#rm -f *_recovery
#
