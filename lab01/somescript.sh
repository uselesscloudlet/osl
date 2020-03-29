#!/bin/bash
cd /mnt/c/Users/cloud/Desktop
mkdir 8091 && cd 8091
mkdir Grigorev && cd Grigorev
touch Dmitri.txt && date > Dmitri.txt
touch filedata.txt && date --date='TZ="Russia/Moscow" next mon' > filedata.txt
cat Dmitri.txt filedata.txt > result.txt
