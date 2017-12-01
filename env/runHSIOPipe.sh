rm /tmp/fromHsio #-f
rm /tmp/toHsio #-f
mkfifo /tmp/fromHsio
mkfifo /tmp/toHsio
chmod 777 /tmp/fromHsio
chmod 777 /tmp/toHsio

# Run without needing separate setup script
LCG_PATH=/cvmfs/sft.cern.ch/lcg/releases/LCG_72a
source $LCG_PATH/gcc/4.9.3/x86_64-slc6/setup.sh
export BOOSTDIR=$LCG_PATH/Boost/1.55.0_python2.7/x86_64-slc6-gcc49-opt/include/boost-1_55

export SCTDAQ_ROOT=/opt/itsdaq/itsdaq_svn_170825
$SCTDAQ_ROOT/bin/hsioPipe --eth p4p1,e0:dd:cc:bb:aa:00 --file /tmp/toHsio,/tmp/fromHsio 
