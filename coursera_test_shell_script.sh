#!/bin/bash
g++ -pipe -O2 -std=c++11 -o suffix_tree_exe suffix_tree.cpp -lm
./suffix_tree_exe < test.txt 
#./suffix_tree_exe < test.txt > testResult.txt
#echo " "

:'
if diff -q "testResult.txt" "testA.txt"
then
        echo " "
        echo "Success!"
else
        echo " "
        echo "\"YOU SHALL NOT PASS\" -Gandolf"
        echo " " 
        diff "testResult.txt" "testA.txt"
fi
'