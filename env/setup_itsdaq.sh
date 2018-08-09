
#LCG_PATH=/cvmfs/sft.cern.ch/lcg/releases/LCG_71
#source $LCG_PATH/gcc/4.9.3/x86_64-slc6/setup.sh
#source $LCG_PATH/ROOT/5.34.24/x86_64-slc6-gcc49-opt/bin/thisroot.sh
#export BOOSTDIR=$LCG_PATH/Boost/1.55.0_python2.7/x86_64-slc6-gcc49-opt/include/boost-1_55

#LCG_PATH=/cvmfs/sft.cern.ch/lcg/releases/LCG_79
#source $LCG_PATH/gcc/4.9.3/x86_64-cc7/setup.sh
#source $LCG_PATH/ROOT/6.04.02/x86_64-cc7-gcc49-opt/bin/thisroot.sh
#export BOOSTDIR=$LCG_PATH/Boost/1.55.0_python2.7/x86_64-cc7-gcc49-opt/include/boost-1_55

LCG_PATH=/cvmfs/sft.cern.ch/lcg/releases/LCG_72a
source $LCG_PATH/gcc/4.9.3/x86_64-slc6/setup.sh
source $LCG_PATH/ROOT/5.34.25/x86_64-slc6-gcc49-opt/bin/thisroot.sh
export BOOSTDIR=$LCG_PATH/Boost/1.55.0_python2.7/x86_64-slc6-gcc49-opt/include/boost-1_55

#export SCTDAQ_ROOT=/opt/itsdaq/itsdaq_svn_170825
export SCTDAQ_ROOT=/opt/itsdaq/itsdaq_git_170825

#export SCTDAQ_VAR=/data/sctvar/
# Need absoute path for SCTDAQ_VAR 
export SCTDAQ_VAR=$HOME/atlas/itsdaq/var   
mkdir -p ${SCTDAQ_VAR}/etc ${SCTDAQ_VAR}/data ${SCTDAQ_VAR}/ps ${SCTDAQ_VAR}/results
