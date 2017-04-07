#!/usr/bin/env python
#
# Parser script to parse all command line inputs in the App
#
import re

def parseAUTHOR(output):
	match = re.match('I, (.*?), have read and understood the course academic integrity policy.\\n', output)

	if match:
	    return "UBITNAME: "+match.group(1)
	else:
	    return "\033[91mFailed to parse: UBITNAME!\033[0m"

def parseIP(output):
	match = re.match('IP:(.*?)\\n', output)

	if match:
	    return "IP address: "+match.group(1)
	else:
	    return "\033[91mFailed to parse: IP address!\033[0m"

def parsePORT(output):
	match = re.match('PORT:(.*?)\\n', output)

	if match:
	    return "Listening port number: "+match.group(1)
	else:
	    return "\033[91mFailed to parse: Listening port number!\033[0m"

def printHosts(host_list):
	display = ""
	for host in host_list:
		host_info = host.split()
		if len(host_info) != 4:
		    host_display = '\033[91mFailed to parse: Client information\033[0m'
		else:
		    host_display = 'ID:%s, Hostname:%s, IP address:%s, Listening Port number:%s' % (host_info[0], host_info[1], host_info[2], host_info[3])

	    display += host_display + '\n'

	return display

def parseLIST(output, type):
	hosts = output.rstrip('\n').split('\n')

	if len(hosts) != 0:
		if type == 'server':
			print "\033[93mRead %d Clients (expected 4):\033[0m" % len(hosts)
		if type == 'client1':
			print "\033[93mRead %d Clients (expected 1):\033[0m" % len(hosts)
		if type == 'client2':
			print "\033[93mRead %d Clients (expected 2):\033[0m" % len(hosts)
		if type == 'client3':
			print "\033[93mRead %d Clients (expected 3):\033[0m" % len(hosts)
		if type == 'client4':
			print "\033[93mRead %d Clients (expected 4):\033[0m" % len(hosts)

		return printHosts(hosts)

	return "\033[91mFailed to parse: List output\033[0m"

def parseRELAYED(output):
	match = re.match('msg from:(.*?), to:(.*?)\\n\[msg\]:(.*?)\\n', output)

	if match: return 'From IP address:%s, To IP address:%s, Message:%s' % (match.group(1), match.group(2), match.group(3))
	else: return "\033[91mFailed to parse: RELAYED message\033[0m"

def parseRECEIVED(output):
	match = re.match('msg from:(.*?)\\n\[msg\]:(.*?)\\n', output)

	if match: return 'From IP address:%s, Message:%s' % (match.group(1), match.group(2))
	else: return "\033[91mFailed to parse: RECEIVED message\033[0m"

def parseSTATISTICS(output):
	hosts = output.rstrip('\n').split('\n')

	if len(hosts) != 0:
		print "\033[93mRead %d Clients (expected 2):\033[0m" % len(hosts)
		display = ""
		for host in hosts:
			host_info = host.split()
			if len(host_info) != 5:
			    host_display = '\033[91mFailed to parse: Client information\033[0m'
			else:
			    host_display = 'ID:%s, Hostname:%s, #Messages-sent:%s, #Messages-received:%s, status:%s' % (host_info[0], host_info[1], host_info[2], host_info[3], host_info[4])

		    display += host_display + '\n'

		return display 
