#!/bin/sh
cd Freenove_RPI_WS281x_Python
python_file=$(which python)
cmd=$(echo sudo ${python_file} setup.py install)
eval $cmd
cd ..
echo "The installation is complete!"

echo "Patching ..."
cd raspi_dev/freenove_code/Patch
source patch_for_bullseye.sh

