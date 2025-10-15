#!/bin/bash

EXE="./main"
OUTDIR="data"

rm -f "$OUTDIR"
mkdir -p "$OUTDIR"

run_case() {
    local file="$1"
    local n="$2"
    local m="$3"
    local q="$4"
    local r="$5"

    TA=$($EXE "$n" "$m" "$q" "$r" "A")
    TB=$($EXE "$n" "$m" "$q" "$r" "B")

    echo "$n $m $TA $TB" >> "$file"
    echo "[$file] n = $n m = $m done"
}

############################
# 3.1 n=1..10001 (5001) step 100
############################
#for ((n=1; n<=5001; n+=100)); do
#    m=$((n*n/10))
#    run_case "$OUTDIR/data1a" "$n" "$m" 1 1000000
#
#    m=$((n*n))
#    run_case "$OUTDIR/data1b" "$n" "$m" 1 1000000
#done

############################
# 3.2 n=101..10001 (5001) step 100
############################
for ((n=101; n<=5001; n+=100)); do
    m=$((100*n))
    run_case "$OUTDIR/data2a" "$n" "$m" 1 1000000

    m=$((1000*n))
    run_case "$OUTDIR/data2b" "$n" "$m" 1 1000000
done

############################
# 3.3 n=10001, m=0..1e7 step 1e5
############################
n=10001
for ((m=0; m<=10000000; m+=100000)); do
    run_case "$OUTDIR/data3a" "$n" "$m" 1 1000000
    run_case "$OUTDIR/data3b" "$n" "$m" 1 1000000
done

############################
# 3.4 n=10001, r=1..200 step 1
############################
n=10001
for ((r=1; r<=200; r+=1)); do
    m=$((n*n))
    run_case "$OUTDIR/data4a" "$n" "$m" 1 "$r"

    m=$((1000*n))
    run_case "$OUTDIR/data4b" "$n" "$m" 1 "$r"
done

