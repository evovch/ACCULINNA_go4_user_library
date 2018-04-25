#!/bin/bash

if [ -z ${PATH+x} ]; then echo "PATH is unset";
else echo "PATH is set to:"; echo $'\t'"${PATH//:/$'\n\t'}"; fi

if [ -z ${ROOTSYS+x} ]; then echo "ROOTSYS is unset";
else echo "ROOTSYS is set to '$ROOTSYS'"; fi

if [ -z ${SIMPATH+x} ]; then echo "SIMPATH is unset";
else echo "SIMPATH is set to '$SIMPATH'"; fi

if [ -z ${FAIRROOTPATH+x} ]; then echo "FAIRROOTPATH is unset";
else echo "FAIRROOTPATH is set to '$FAIRROOTPATH'"; fi

if [ -z ${LD_LIBRARY_PATH+x} ]; then echo "LD_LIBRARY_PATH is unset";
else echo "LD_LIBRARY_PATH is set to:"; echo $'\t'"${LD_LIBRARY_PATH//:/$'\n\t'}"; fi

if [ -z ${GO4SYS+x} ]; then echo "GO4SYS is unset";
else echo "GO4SYS is set to '$GO4SYS'"; fi
