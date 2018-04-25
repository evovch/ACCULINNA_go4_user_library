#!/bin/bash

SOFTDIR=/home/evovch/soft

cd ${SOFTDIR}/go4-5.3.0_fairsoftroot/
. ./go4login
cd -

cd ${SOFTDIR}/FairSoft_install/bin/
. ./thisroot.sh
cd -
