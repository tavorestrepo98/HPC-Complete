#! /bin/bash
for j in 100 200 300 400 500 1000 2000 
do
for i in  {1..10}; do
./fork $j >> out$j
done;
done;
