#!/bin/bash

if [ -d "build" ]; then
	rm -rf build/*
fi

if [ -d "install" ]; then
	rm -rf install/*
fi

# Cleanup analysis results ====================================================

# Remove textual output if it already exists from previous analysis runs
if [ -d "textoutput" ]; then
	rm -fv textoutput/*.txt
fi

echo -e "\e[1m\e[32mCleanup finished.\e[0m"
