#!/bin/bash

# Cleanup compile results =====================================================

unlink libSetupConfig.so
#unlink libSetupConfig_rdict.pcm
#unlink libSetupConfig.rootmap
unlink setup_config_test

unlink libUserAnalysis.so
unlink libUserAnalysis_rdict.pcm
unlink libUserAnalysis.rootmap

unlink libSetupConfigCppWrapper.so
unlink libSetupConfigCppWrapper_rdict.pcm
unlink libSetupConfigCppWrapper.rootmap

if [ -d "build" ]; then
	rm -rf build/*
fi

# Cleanup analysis results ====================================================

# Remove textual output if it already exists from previous analysis runs
if [ -d "textoutput" ]; then
	rm -fv textoutput/*.txt
fi

echo -e "\e[1m\e[32mCleanup finished.\e[0m"
