#!/bin/sh

if [ $1 = "NAME" ]; then
    adb shell dumpsys meminfo -a ${2} | tee -a ./data/report.txt
elif [ $1 = "PID" ]; then
    adb shell dumpsys meminfo ${2} | tee -a ./data/report.txt
else
    echo "no support option: ${1}"
    exit 0
fi