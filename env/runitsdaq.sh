echo Running ITS DAQ

if [ "x${BASH_ARGV[0]}" = "x" ]; then
  echo "Using SCTDAQ_ROOT=$SCTDAQ_ROOT (from env)"
else
  which realpath 2> /dev/null > /dev/null && export SCTDAQ_ROOT=$(realpath $(dirname ${BASH_ARGV[0]}))
  which realpath 2> /dev/null > /dev/null || export SCTDAQ_ROOT=$(dirname ${BASH_ARGV[0]})
  if [ "${SCTDAQ_ROOT}" = "." ]; then
    export SCTDAQ_ROOT=$(pwd)
  fi
  echo "Using SCTDAQ_ROOT=$SCTDAQ_ROOT (from script directory)"
fi

RUN_ROOT6=no

if [[ "$ROOTSYS" == "" || "$ROOTSYS" == /usr ]] ; then
  root-config --version | grep "^6.*" > /dev/null && RUN_ROOT6=yes
  export ROOTSYS=/usr
elif [ "x$(echo $ROOTSYS | sed 's:.*ROOT/\(.\).*:\1:')" = "x6" ]; then
  RUN_ROOT6=yes
fi

RUN_DIR=$SCTDAQ_ROOT

if [ $RUN_ROOT6 == yes ]; then
  echo Using root 6 setup
  RUN_DIR=$SCTDAQ_ROOT/root6

  # Macros expect to be able to refer to things from top directory
  export ROOT_INCLUDE_PATH=$SCTDAQ_ROOT
fi

[[ "$SCTDAQ_VAR" == "" ]] && echo -e '\033[31mPlease set SCTDAQ_VAR to point to config area\033[39m'

echo "Now running Stavelet macro in ROOT"
(cd $RUN_DIR; $ROOTSYS/bin/root -l Stavelet.cpp)
