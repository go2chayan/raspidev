#!/bin/sh
cd ~/Freenove_RPI_WS281x_Python
python_file=$(which python)
cmd=$(echo sudo ${python_file} setup.py install)
eval $cmd
echo "The installation is complete!"
