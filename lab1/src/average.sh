#!/bin/bash

#$1=2
#$2=5
#$3=1
s=0
N=8
for ((i=1; i<N+1; i++))
do
s=$(($s+$i));
done
res=$(($s/$N))
echo $res
echo $N

