#!/bin/bash

if [ -z ${GO4SYS+x} ]; then
	echo "GO4SYS is unset. Aborting.";
	exit;
else
	echo "GO4SYS is set to '$GO4SYS'";
fi

# Please specify these variables to your needs ================================

INPUTLMDDIR=/home/evovch/Downloads/FLNR_data/exp201805
OUTPUTROOTDIR=/home/evovch/Downloads/FLNR_data/exp201805

SETUPFILE=./usr/setup_exp201805.xml
INPUTFILENAME=run05_0016.lmd

NEVENTS=10000

TEXTERRFILE=textoutput/err.txt  # Default: textoutput/err.txt
#TEXTERRFILE=/dev/null    # suppress error stream. Saves a little bit of hdd space

TEXTOUTFILE=textoutput/out.txt  # Default: textoutput/out.txt # No reason to suppress

# =============================================================================

INPUTFILE=${INPUTLMDDIR}/${INPUTFILENAME}
OUTPUTFILE=${OUTPUTROOTDIR}/${INPUTFILENAME}.root
AUTOSAVEFILE=${OUTPUTROOTDIR}/${INPUTFILENAME}_asf.root

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

echo -e "\e[1m\e[32mAnalysis started.\e[0m"

# if NEVENT == 0 ...

${GO4SYS}/bin/go4analysis -v -lib libUserAnalysis.so -number ${NEVENTS} -asf ${AUTOSAVEFILE} -file ${INPUTFILE} -args ${OUTPUTFILE} ${SETUPFILE} >> ${TEXTOUTFILE} 2> ${TEXTERRFILE}
# -number 1000

echo -e "\e[1m\e[32mAnalysis finished.\e[0m"
