test_dir="$1"
for name in TIME_MEASUREMENT.txt FIFO_1.txt PSJF_2.txt RR_3.txt SJF_4.txt; do
    echo -e "\e[33mdmesg --clear\e[0m"
    dmesg --clear
    echo -e "\e[33m./main < $test_dir/$name\e[0m"
    ./main < "$test_dir/$name"
    echo -e "\e[33mdmesg | grep Project1\e[0m"
    dmesg | grep 'Project1'
    echo ''
    sleep 3
done