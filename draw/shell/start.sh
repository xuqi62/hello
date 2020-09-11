#!/bin/sh

set -e

if [ $# -lt 2 ]; then
	echo "invalid argument!!!"
	echo "usage: ./cut.sh process_name"
	exit 0
fi

if [ $1 = "-a" ]; then
	echo "catch process by name: ${2}"
	watch -n 0.01 ./shell/mm.sh NAME ${2}
elif [ $1 = "-p" ]; then
	echo "catch process by pid: ${2}"
	watch -n 0.01 ./shell/mm.sh PID ${2}
else
	echo "not support option: ${1}"
	exit 0
fi
