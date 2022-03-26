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
3. Run the build:
```bash
source build.sh
```
4. Edit the `/boot/config.txt` file:
```bash
sudo nano /boot/config.txt
```
5. Add # before camera_auto_detect=1 -> # camera_auto_detect=1
6. Add f in dtoverlay=vc4-kms-v3d    -> dtoverlay=vc4-fkms-v3d
7. Add following in the end. 
```
start_x=1
gpu_mem=128
```
8. Add following in the end
```
hdmi_force_hotplug=1 
hdmi_ignore_edid=0xa5000080 
hdmi_group=2
hdmi_mode=82
```
9. Finally press `Ctrl+O`, `Enter`, then `Ctrl+X`


