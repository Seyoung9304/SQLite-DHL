name1="result_breakdown_$1"
name2="result_oper_ratio_$1"
name3="result_tpcc_$1"
echo "breakdown"
sh breakdown.sh $name1
echo "oper_ratio"
sh oper_ratio.sh $name2

echo "tail"

sh tail.sh
echo "tpcc"
cd tpcc
sh tpcc.sh $name3
cd ..
