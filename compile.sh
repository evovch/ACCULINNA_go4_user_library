#!/bin/bash

# Actually these checks may be an overkill.
# There are basically the same checks done by CMake according to the
# instructions in the top CMakeLists.txt

if [ -z ${GO4SYS+x} ]; then
	echo "No Go4 found! GO4SYS is unset. Please run thisroot.sh";
	exit;
else
	GO4VERSION=`$GO4SYS/bin/go4-config --version`
	echo "Found Go4" $GO4VERSION "in GO4SYS='$GO4SYS'";
fi

if [ -z ${ROOTSYS+x} ]; then
	echo "No ROOT found! ROOTSYS is unset. Please run thisroot.sh";
	exit;
else
	echo "Found ROOT" `root-config --version` "in ROOTSYS='$ROOTSYS'";
fi

# TODO activate/deactivate
./doclean.sh

if [ ! -d "build" ]; then
	mkdir build
fi

cd build
cmake ../
make -j
cd -

ln -s build/setupconfig/libSetupConfig.so
#ln -s build/setupconfig/libSetupConfig_rdict.pcm
#ln -s build/setupconfig/libSetupConfig.rootmap
ln -s build/setupconfig/setup_config_test

ln -s build/useranalysis/libUserAnalysis.so
ln -s build/useranalysis/libUserAnalysis_rdict.pcm
ln -s build/useranalysis/libUserAnalysis.rootmap

ln -s build/setupconfigcppwrapper/libSetupConfigCppWrapper.so
ln -s build/setupconfigcppwrapper/libSetupConfigCppWrapper_rdict.pcm
ln -s build/setupconfigcppwrapper/libSetupConfigCppWrapper.rootmap

echo -e "\e[1m\e[32mCompilation finished.\e[0m"
