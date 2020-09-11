#!/bin/sh

cat ./data/report.txt | grep "TOTAL:" | cut -f 2 -d ":" | sed 's/^[ \t]*//g' | cut -f 1 -d " " > ./data/output.csv
cat ./data/report.txt | grep "Graphics:" | cut -f 2 -d ":" > ./data/graphics_output.csv
cat ./data/report.txt | grep "Native Heap:" | cut -f 2 -d ":" > ./data/native_output.csv
cat ./data/report.txt | grep "Code:" | cut -f 2 -d ":" > ./data/code_output.csv
# et ./data/output.csv &
python2 ./shell/draw.py &
