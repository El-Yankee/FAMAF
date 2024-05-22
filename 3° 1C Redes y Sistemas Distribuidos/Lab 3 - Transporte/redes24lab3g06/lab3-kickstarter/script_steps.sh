#! /bin/sh
RUNS=$(./lab3-kickstarter -q runnumbers)
for i in $RUNS; do
    ./lab3-kickstarter -u Cmdenv -r $i 
done
