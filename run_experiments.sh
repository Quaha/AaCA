#!/bin/bash

g++ -O3 -std=c++23 tests.cpp -o tests



# === 3.1 ===
echo "Running 3.1a and 3.1b..."
q=1
r=1000000
for ((n=1; n<=4001; n+=100)); do
    # 3.1a: m = n^2 / 10
    m=$(( n * n / 10 ))
    ./tests $n $m $q $r G
    tA=$(./tests $n $m $q $r A)
    tB=$(./tests $n $m $q $r B)
    echo "$n $tA $tB" >> results/exp3_1a.txt

    # 3.1b: m = n^2
    m=$(( n * n ))
    ./tests $n $m $q $r G
    tA=$(./tests $n $m $q $r A)
    tB=$(./tests $n $m $q $r B)
    echo "$n $tA $tB" >> results/exp3_1b.txt

    echo $(((n - 1) / 100))
done

# === 3.2 ===
echo "Running 3.2a and 3.2b..."
for ((n=101; n<=5001; n+=100)); do
    # 3.2a: m = 100 * n
    m=$(( 100 * n ))
    ./tests $n $m $q $r G
    tA=$(./tests $n $m $q $r A)
    tB=$(./tests $n $m $q $r B)
    echo "$n $tA $tB" >> results/exp3_2a.txt

    # 3.2b: m = 1000 * n
    m=$(( 1000 * n ))
    ./tests $n $m $q $r G
    tA=$(./tests $n $m $q $r A)
    tB=$(./tests $n $m $q $r B)
    echo "$n $tA $tB" >> results/exp3_2b.txt

    echo $(((n - 1) / 100))
done


# === 3.3 ===
echo "Running 3.3..."
n=10001
for ((m=0; m<=2500000; m+=100000)); do
    ./tests $n $m $q $r G
    tA=$(./tests $n $m $q $r A)
    tB=$(./tests $n $m $q $r B)
    echo "$m $tA $tB" >> results/exp3_3.txt

    echo $((m / 100000))
done



# === 3.4 ===
echo "Running 3.4a and 3.4b..."
q=1
n=1501
for ((r=1; r<=200; r+=1)); do
    # 3.4a: m = n^2
    m=$(( n * n ))
    ./tests $n $m $q $r G
    tA=$(./tests $n $m $q $r A)
    tB=$(./tests $n $m $q $r B)
    echo "$r $tA $tB" >> results/exp3_4a.txt

    # 3.4b: m = 1000 * n
    m=$(( 1000 * n ))
    ./tests $n $m $q $r G
    tA=$(./tests $n $m $q $r A)
    tB=$(./tests $n $m $q $r B)
    echo "$r $tA $tB" >> results/exp3_4b.txt

    echo $((r))
done
