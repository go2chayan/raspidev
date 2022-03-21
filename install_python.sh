sudo apt-get update \
    && sudo apt-get upgrade -y \
    && sudo apt-get install -y wget \
    && sudo wget -q http://repo.continuum.io/miniconda/Miniconda3-latest-Linux-armv7l.sh \
    && bash ./Miniconda3-latest-Linux-armv7l.sh -b -p ~/bin/conda \
    && ~/bin/conda/bin/conda config --add channels rpi \
    && ~/bin/conda/bin/conda create -y -n raspi_venv python=3.7 pip \
    && rm -f ./Miniconda3-latest-Linux-armv7l.sh \
    && echo "export PATH=\"~/bin/conda/bin/:\$PATH\"" >> ${HOME}/.bashrc