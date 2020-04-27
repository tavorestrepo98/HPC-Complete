for j in {1..10}; do
for i in 100 200 300 400 500 1000 2000 
do
./hilos $i >> out$i
done;
done;
