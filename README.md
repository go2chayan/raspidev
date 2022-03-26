# Raspi-Dev

## Virtual Environment
In `raspberry-pi` use the virtual environment as below:
```bash
python -m venv ~/env
```
Activate virtual environment:
```bash
source ~/env/bin/activate
```
Deactivate virtual environment:
```bash
deactivate
```

## Setting up raspberry pi
To install the package for Raspberry pi,

```bash
python setup.py build
python setup.py install
```

## Setting up Freenove 4WD Smart Car

1. Enable I2C from Raspberry Pi Configuration
2. Try `i2cdetect -y 1`. The smart car board has two chips, PCF8591 and PCA9685. Their I2C addresses are 0X48 and 0X40 respectively.

To install the Freenove 4WD car kit sdk, run the following
```bash
build.sh
```

