#!/bin/bash

file_len=0
filename="email.log"

mapfile lines < $filename


while [ $file_len -eq $file_len ]; do 

while [ $file_len -eq ${#lines[@]} ]; do 
    mapfile lines < $filename
done
file_len=${#lines}
for i in "${!lines[@]}"; do
    line=${lines[$i]}
    line=$(echo "$line" | tr -d '\n' | tr -d '\r' | tr -d '\t')
    line="${line%% }"
    line="${line# }"
    lines[$i]=$line
done

i=$(( file_len+1 ))
while [ $i -lt ${#lines[@]} ]; do
    j=$i
    time=${lines[$j]}
    j=$(( j + 1 ))
    from=${lines[$j]}
    j=$(( j + 1 ))
    to=${lines[$j]}
    j=$(( j + 1 ))
    message=${lines[$j]}
    i=$(( j+1 ))
    
    if [[ "${to:6:5}" = "enemy" ]]; then
        echo Alert
    fi
done
file_len=${#lines[@]}


done