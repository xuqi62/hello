#!/bin/sh

if [ $# -lt 2 ]; then
	echo "invalid argument!!!"
	echo "usage: ./run_catcher.sh -a process_name"
	echo "       ./run_catcher.sh -p pid"
	exit 0
fi

echo "start catcher"

if [ ! -d "data-backup" ]; then
	rm data-backup
	mkdir data-backup
fi

if [ ! -f "data-backup/backup.txt" ]; then
	touch data-backup/backup.txt
	echo "0" > data-backup/backup.txt
fi

if [ -f data/report.txt ]; then
	number=`cat data-backup/backup.txt`
	echo $number

	step=1
	number=`expr $number + $step`
	echo $number

	rm data-backup/backup.txt
	echo "${number}" > data-backup/backup.txt
	mkdir data-backup/${number}/
	cp data/* data-backup/${number}

	rm data/*
fi

sh ./shell/start.sh ${1} ${2}
