#!/bin/bash

if [ -z ${GO4SYS+x} ]; then
	echo "GO4SYS is unset. Aborting.";
	exit;
else
	echo "GO4SYS is set to '$GO4SYS'";
fi

# Please specify these variables to your needs ================================

INPUTLMDDIR=/home/evovch/Downloads/FLNR_data/exp201803

if [ ! -d "${INPUTLMDDIR}" ]; then
	echo -e "Directory ${INPUTLMDDIR} does not exist. Aborting."
	exit;
fi

OUTPUTROOTDIR=/home/evovch/Downloads/FLNR_data/exp201803/result

# TODO check that ${OUTPUTROOTDIR} exists ???

SETUPFILE=./usr/setup2_exp201803.xml

# TODO check that ${SETUPFILE} exists

INPUTFILENAME=run13_0037.lmd

NEVENTS=0 # Specify 0 to process the full file

TEXTERRFILE=textoutput/err.txt  # Default: textoutput/err.txt
#TEXTERRFILE=/dev/null    # suppress error stream. Saves a little bit of hdd space

TEXTOUTFILE=textoutput/out.txt  # Default: textoutput/out.txt # No reason to suppress

# =============================================================================

INPUTFILE=${INPUTLMDDIR}/${INPUTFILENAME}
OUTPUTFILE=${OUTPUTROOTDIR}/${INPUTFILENAME}.root
AUTOSAVEFILE=${OUTPUTROOTDIR}/${INPUTFILENAME}_asf.root
DATETIME=`date`

# Cleanup analysis results ====================================================

# Remove output file if it already exists from previous analysis runs
rm -fv ${OUTPUTFILE}
# Remove autosave file if it already exists from previous analysis runs
rm -fv ${AUTOSAVEFILE}
# Remove textual output if it already exists from previous analysis runs
if [ -d textoutput ]; then
	rm -fv textoutput/*.txt
else
	mkdir textoutput
fi

# =============================================================================

ANARUNINFO="========================== ANALYSIS RUN INFO ===================================
Current date and time                      ${DATETIME}
Processing input file                      ${INPUTFILE}
acording to the setup configuration file   ${SETUPFILE}
into output file                           ${OUTPUTFILE}
with autosave file                         ${AUTOSAVEFILE}
Text output file:                          ${TEXTOUTFILE}
Text error file:                           ${TEXTERRFILE}
Text summary files:                        textoutput/*.txt
================================================================================"

cat > ${TEXTOUTFILE} << EOF
${ANARUNINFO}
EOF

echo -e "\e[1m\e[34m${ANARUNINFO}\e[0m"

echo -e "\e[1m\e[32m"`date` "Analysis started.\e[0m"

if [ ${NEVENTS} -eq 0 ]; then
	${GO4SYS}/bin/go4analysis -v -lib libAccDaqUserAnalysis.so -asf ${AUTOSAVEFILE} -file ${INPUTFILE} -args ${OUTPUTFILE} ${SETUPFILE} >> ${TEXTOUTFILE} 2> ${TEXTERRFILE}
else
	${GO4SYS}/bin/go4analysis -v -lib libAccDaqUserAnalysis.so -number ${NEVENTS} -asf ${AUTOSAVEFILE} -file ${INPUTFILE} -args ${OUTPUTFILE} ${SETUPFILE} >> ${TEXTOUTFILE} 2> ${TEXTERRFILE}
fi

echo -e "\e[1m\e[32m"`date` "Analysis finished.\e[0m"
