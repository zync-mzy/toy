#!/bin/sh

function find_suid_bash_backdoor {
	echo
	echo "find_suid_bash_backdoor"

	for x in `find / -perm /4000 2>/dev/null`; do
		strings $x | grep bash_copyright > /dev/null
		if [ $? -eq 0 ]; then
			echo "$x contains bash_copyright with suid set"
		fi	
	done
}

function find_pam_backdoor {
	echo
	echo "find_pam_backdoor"

	pam_rpm=`rpm -qf /lib/security/pam_unix.so 2>/dev/null`
	if [ -z $pam_rpm ]; then
		pam_rpm=`rpm -qf /lib64/security/pam_unix.so`
	fi

	show=false
	for x in `rpm --verify $pam_rpm | grep \.so`; do
		if $show; then
			echo "$x md5 changed"
			show=false
			continue
		fi

		if expr match $x '..5.*' >/dev/null; then
			show=true
		fi
	done
	echo $verify
}

function find_port_knock_backdoor {
	echo
	echo "find_port_knock_backdoor"

	whitelist="/usr/sbin/libvirtd|123456"
	for x in `ls /proc/`; do
		if [ $x -gt 0 2>/dev/null ];then
			grep pcap /proc/$x/maps >/dev/null 2>&1 || continue
			proc=`ls -l /proc/$x/exe | awk '{print $NF}'`
			echo $whitelist | grep $proc >/dev/null 2>&1
			if [ $? -ne 0 ]; then
				 echo "$proc(pid: $x) is using pcap"
			else
				 echo "$proc(pid: $x) is using pcap, ignore as it's in whitelist"
			fi
		fi
	done
}

function find_icmp_backdoor {
	echo
	echo "find_icmp_backdoor"

	whitelist="/usr/sbin/NetworkManager|123"
	for x in `netstat -lwp | grep icmp | awk '{print $NF}'`; do
		pid=`echo $x | cut -d/ -f1`
		proc=`ls -l /proc/$pid/exe | awk '{print $NF}'`
		echo $whitelist | grep $proc >/dev/null 2>&1
		if [ $? -ne 0 ]; then
			 echo "$proc(pid: $pid) is listening icmp"
		else
			 echo "$proc(pid: $pid) is listening icmp, ignore as it's in whitelist"
		fi
	done
}

find_suid_bash_backdoor
find_pam_backdoor
find_port_knock_backdoor
find_icmp_backdoor
