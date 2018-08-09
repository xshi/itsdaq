#!/usr/bin/env bash

# Main driver to run ITSDAQ 
# Author SHI Xin <shixin@ihep.ac.cn>
# Created [2017-12-01 Fri 15:12] 

usage() {
    printf "NAME\n\trun.sh - Main driver to run ITSDAQ\n"
    printf "\nSYNOPSIS\n"
    printf "\n\t%-5s\n" "./run.sh [OPTION]" 
    printf "\n\n" 
}


cd $SCTDAQ_ROOT
. RUNITSDAQ.sh 


