# OS_Proj1
Operating System Project 1, Spring 2020, National Taiwan University

## How to execute the program
```
make
sudo dmesg --clear && sudo ./main <input.txt >output.txt
```

Sample output:
```
P1 21784
P2 21785
P3 21786
P4 21787
P5 21788
```

## How to check kernel messages
```
dmesg
```

Sample output:
```
[Project1] 21784 1588063172.083862650 1588063172.918043147
[Project1] 21785 1588063172.920598970 1588063173.738268054
[Project1] 21786 1588063173.738555479 1588063174.577245686
[Project1] 21787 1588063174.577832047 1588063175.430213123
[Project1] 21788 1588063175.430564035 1588063176.253668599
```

## How to compile the program with debug message
```
make debug
```
The execution method is the same as above.
