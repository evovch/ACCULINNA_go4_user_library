#!/bin/bash

SOFTDIR=/home/evovch/soft

cd ${SOFTDIR}/go4_trunk/
. ./go4login
cd -

cd ${SOFTDIR}/FairSoft_install/bin/
. ./thisroot.sh
cd -

# https://stackoverflow.com/questions/59895/getting-the-source-directory-of-a-bash-script-from-within
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )"

export LD_LIBRARY_PATH=${DIR}/install/lib:$LD_LIBRARY_PATH
