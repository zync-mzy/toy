#!/bin/sh

# suid shell：扫描系统带suid属性的文件，用strings查看文件内容，如果有bash_copyright字样（可以有更多判定条件），判定是shell，发出提醒
# port knocking：扫描系统进程的maps文件，对用到libpcap的进程发出提醒，可以加白名单过滤
# icmp后门：通过netstat -lwp | grep icmp扫描监听icmp的进程，发出提醒，可以加白名单过滤
# pam后门：通过rpm自带的验证命令，验证pam包的完整性，对异常文件发出提醒

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
	for x in `ls /proc/`; do
		if [ $x -gt 0 ] 2>/dev/null ;then
			for sockid in `ls -l /proc/$x/fd 2>/dev/null | grep socket | cut -d[ -f2 | cut -d] -f1`; do
				match=false
				port=`cat /proc/net/raw | grep $sockid | cut -d: -f3 | cut -d' ' -f1`
				if [ $((0x$port)) -eq 1 ] 2>/dev/null; then
					match=true
				fi
				port=`cat /proc/net/raw6 | grep $sockid | cut -d: -f3 | cut -d' ' -f1`
				if [ $((0x$port)) -eq 58 ] 2>/dev/null; then
					match=true
				fi
				proc=`ls -l /proc/$x/exe | awk '{print $NF}'`
				echo $whitelist | grep $proc >/dev/null 2>&1
				if [ $? -ne 0 ] && $match; then
					 echo "$proc(pid: $x) is listening icmp"
				elif $match; then
					 echo "$proc(pid: $x) is listening icmp, ignore as it's in whitelist"
				fi
			done
		fi
	done
}

find_suid_bash_backdoor
find_pam_backdoor
find_port_knock_backdoor
find_icmp_backdoor
