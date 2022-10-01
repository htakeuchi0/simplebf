#!/bin/bash
# Install Doxygen for generating the documentation
doxygen --version > /dev/null 2>&1
if [ $? -ne 0 ]; then 
	if [ -f "/etc/centos-release" ]; then
		# CentOS
		sudo dnf install dnf-plugins-core -y
		sudo dnf config-manager --set-enabled PowerTools
		sudo dnf install doxygen -y
		sudo dnf install graphviz -y
	elif [ -f "/etc/lsb-release" ]; then
		grep -q 'DISTRIB_ID=Ubuntu' /etc/lsb-release
		if [ $? = 0 ]; then
			# Ubuntu
			sudo apt install doxygen -y
			sudo apt install graphviz -y
		else
			echo "未対応の環境です．"
			exit 1
		fi
	else 
		echo "未対応の環境です．"
		exit 1
	fi
fi
