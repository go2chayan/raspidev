#!/bin/sh
current_file=$0
cd "$(dirname "$current_file")"

#sudo cp /boot/config.txt /boot/config-backup.txt
#sudo cp ./config.txt /boot/config.txt

issue_date=$(cat /etc/rpi-issue|grep -o "[0-9]*-[0-9]*-[0-9]*")
threshold_date="2021-10-30"

if [[ "$issue_date" > "$threshold_date"]] ;
then
	sudo cp /usr/lib/arm-linux-gnueabihf/libmmal.so /usr/lib/arm-linux-gnueabihf/libmmal-backup.so.backup
	sudo cp ./libmmal.so /usr/lib/arm-linux-gnueabihf/libmmal.so
	sudo cp /opt/vc/lib/libmmal.so /opt/vc/lib/libmmal.so.bak 
	sudo cp ./libmmal.so /opt/vc/lib/libmmal.so
	echo "patch complete!"
else
	echo "patch not required"

