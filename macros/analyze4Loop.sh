#!/bin/bash

for (( i=1; i < 10; i++ ))
do
root -l -q "convert.C($i)"
done
