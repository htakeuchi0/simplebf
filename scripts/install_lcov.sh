#!/bin/bash

# Install LCOV for the coverage testing
lcov --version > /dev/null 2>&1
if [ $? -ne 0 ]; then 
	if [ -f "/etc/centos-release" ]; then
		# CentOS
		sudo dnf install perl -y
		wget http://downloads.sourceforge.net/ltp/lcov-1.14-1.noarch.rpm
		rpm -ivh lcov-1.14-1.noarch.rpm
		rm lcov-1.14-1.noarch.rpm -y
	elif [ -f "/etc/lsb-release" ]; then
		grep -q 'DISTRIB_ID=Ubuntu' /etc/lsb-release
		if [ $? = 0 ]; then
			# Ubuntu
			sudo apt install lcov -y
		else
			echo "未対応の環境です．"
			exit 1
		fi
	else 
		echo "未対応の環境です．"
		exit 1
	fi
fi
