!/usr/bin/env bash

# Main setup for ITSDAQ
# Author SHI Xin <shixin@ihep.ac.cn>
# Created [2017-11-30 Thu 15:23] 


usage() {
    printf "NAME\n\tsetup.sh - Main setup for ITSDAQ\n"
    printf "\nSYNOPSIS\n"
    printf "\n\t%-5s\n" "./setup.sh [OPTION]" 
    printf "\nOPTIONS\n" 
    printf "\n\t%-5s  %-40s\n"  "1"  "DAQLoad RAL_DLRH5 on RAL-HEPLNW0185" 

    printf "\n\n" 
}

if [[ $# -eq 0 ]]; then
    usage
    echo "Please enter your option: "
    read option
else
    option=$1    
fi


case $option in 
    1) echo "Setting up DAQLoad RAL_DLRH5 on RAL-HEPLNW0185 ..." 
    # validate the sudo first
   	sudo -v 
	cd /opt/itsdaq/
	sudo ./runHSIOPipe.sh &
	cd -  
	source ./env/setup_itsdaq.sh
;;

esac
