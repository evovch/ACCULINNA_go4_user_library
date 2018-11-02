#!/bin/bash

# Actually these checks may be an overkill.
# There are basically the same checks done by CMake according to the
# instructions in the top CMakeLists.txt

#if [ -z ${GO4SYS+x} ]; then
#	echo "No Go4 found! GO4SYS is unset. Please run thisroot.sh";
#	exit;
#else
#	GO4VERSION=`$GO4SYS/bin/go4-config --version`
#	echo "Found Go4" $GO4VERSION "in GO4SYS='$GO4SYS'";
#fi

if [ -z ${ROOTSYS+x} ]; then
	echo "No ROOT found! ROOTSYS is unset. Please run thisroot.sh";
	exit;
else
	echo "Found ROOT" `root-config --version` "in ROOTSYS='$ROOTSYS'";
fi

# TODO activate/deactivate
#./doclean.sh

if [ ! -d "build" ]; then
	mkdir build
fi

if [ ! -d "install" ]; then
	mkdir install
fi

cd build
cmake ../ -DCMAKE_INSTALL_PREFIX=../install # -DREADER_MODE=ON
make -j
make install
cd -

echo -e "\e[1m\e[32mCompilation finished.\e[0m"
