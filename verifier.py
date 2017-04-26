import argparse

parser = argparse.ArgumentParser(description='CSE 489/589 PA1: Verifier')

requiredArgs = parser.add_argument_group('required named arguments')
requiredArgs.add_argument('-p', '--path', dest='path', type=str, nargs=1, help='path to your binary', required=True)

args = parser.parse_args()

def procStatus(pid):
    for line in open("/proc/%d/status" % pid).readlines():
        if line.startswith("State:"):
            return line.split(":",1)[1].strip().split(' ')[0]
    return None

if __name__ == "__main__":

	import sys
	import subprocess
	import time
	import os
	import socket
	import re

	def extractOutputSuccess(command, logfile_path):
		logfile = open(logfile_path, 'r')
		log_output = logfile.read()
		logfile.close()

		matches = re.compile('\['+command+':SUCCESS\]\\n(.*?)\['+command+':END\]\\n', re.DOTALL).search(log_output)

		if not matches:
			print "\033[91mNo output detected: Check format/syntax of output.\033[0m"
			return None
		return matches.group(1)

	from assignment1_parser import *

	logfile_path = os.path.dirname(args.path[0])+"/logs/assignment_log_"+socket.getfqdn().split('.')[0]

	def parseOutput(output, parse_func):
		if output:
			sys.stdout.write('\033[94m'+output+'\033[0m')
			sys.stdout.flush()
			print "---------------------------------------"
			print "I parsed the following from the output:"
			print '\033[92m'+parse_func()+'\033[0m'

	#Application Startup
    print
    print '\033[33m'+"Application Startup ..."+'\033[0m',
    sys.stdout.flush()
    s_or_c = 's'
    port = 4242
    command = args.path[0]+" "+s_or_c+" "+str(port)
    process = subprocess.Popen(command, shell=True, stdout=open(os.devnull, 'w'), stderr=subprocess.STDOUT)
    time.sleep(4)
    status = procStatus(process.pid)
    if status == 'R' or status == 'S':
      os.system('kill -9 '+str(process.pid))
      print "PASS"
    else:
      print "FAIL"
      exit()


    #AUTHOR
    print
    print '\033[33m'+"AUTHOR ..."+'\033[0m',
    sys.stdout.flush()
    s_or_c = 's'
    port = 4242
    expect_command = "expect -f author.exp "+args.path[0]+" "+s_or_c+" "+str(port)
    process = subprocess.Popen(expect_command, shell=True, stdout=open(os.devnull, 'w'), stderr=subprocess.STDOUT)
    time.sleep(2)

    print
    print "I got the following output:"
    output = extractOutputSuccess("AUTHOR", logfile_path+"_"+str(port))
    parseOutput(output, lambda: parseAUTHOR(output))

    #IP
    print
    print '\033[33m'+"IP ..."+'\033[0m',
    sys.stdout.flush()
    s_or_c = 's'
    port = 4242
    expect_command = "expect -f ip.exp "+args.path[0]+" "+s_or_c+" "+str(port)
    process = subprocess.Popen(expect_command, shell=True, stdout=open(os.devnull, 'w'), stderr=subprocess.STDOUT)
    time.sleep(3)

    print
    print "I got the following output:"
    output = extractOutputSuccess("IP", logfile_path+"_"+str(port))
    parseOutput(output, lambda: parseIP(output))


    #PORT
    print
    print '\033[33m'+"PORT ..."+'\033[0m',
    sys.stdout.flush()
    s_or_c = 's'
    port = 4242
    expect_command = "expect -f port.exp "+args.path[0]+" "+s_or_c+" "+str(port)
    process = subprocess.Popen(expect_command, shell=True, stdout=open(os.devnull, 'w'), stderr=subprocess.STDOUT)
    time.sleep(3)

    print
    print "I got the following output:"
    output = extractOutputSuccess("PORT", logfile_path+"_"+str(port))
    parseOutput(output, lambda: parsePORT(output))

    #LIST OF CLIENTS
    print
    print '\033[33m'+"LIST ..."+'\033[0m',
    sys.stdout.flush()
    s_or_c = 's'
    port = 4242
    expect_command = "expect -f list_server.exp "+args.path[0]+" "+s_or_c+" "+str(port)
    server = subprocess.Popen(expect_command, shell=True, stdout=open(os.devnull, 'w'), stderr=subprocess.STDOUT)

    s_or_c = 'c'
    port = 1111
    expect_command = "expect -f list_client.exp "+args.path[0]+" "+s_or_c+" "+str(port)
    client_1 = subprocess.Popen(expect_command, shell=True, stdout=open(os.devnull, 'w'), stderr=subprocess.STDOUT)
    time.sleep(1)
    port = 1212
    expect_command = "expect -f list_client.exp "+args.path[0]+" "+s_or_c+" "+str(port)
    client_2 = subprocess.Popen(expect_command, shell=True, stdout=open(os.devnull, 'w'), stderr=subprocess.STDOUT)
    time.sleep(1)
    port = 1313
    expect_command = "expect -f list_client.exp "+args.path[0]+" "+s_or_c+" "+str(port)
    client_3 = subprocess.Popen(expect_command, shell=True, stdout=open(os.devnull, 'w'), stderr=subprocess.STDOUT)
    time.sleep(1)
    port = 1414
    expect_command = "expect -f list_client.exp "+args.path[0]+" "+s_or_c+" "+str(port)
    client_4 = subprocess.Popen(expect_command, shell=True, stdout=open(os.devnull, 'w'), stderr=subprocess.STDOUT)
    time.sleep(12)

    os.system('kill -9 '+str(client_1.pid))
    os.system('kill -9 '+str(client_2.pid))
    os.system('kill -9 '+str(client_3.pid))
    os.system('kill -9 '+str(client_4.pid))

    print
    print "Output Received:"
    print "-On Server-"
    output = extractOutputSuccess("LIST", logfile_path+"_4242")
    parseOutput(output, lambda: parseLIST(output, 'server'))

    print "-On Client 1-"
    output = extractOutputSuccess("LIST", logfile_path+"_1111")
    parseOutput(output, lambda: parseLIST(output, 'client1'))

    print "-On Client 2-"
    output = extractOutputSuccess("LIST", logfile_path+"_1212")
    parseOutput(output, lambda: parseLIST(output, 'client2'))

    print "-On Client 3-"
    output = extractOutputSuccess("LIST", logfile_path+"_1313")
    parseOutput(output, lambda: parseLIST(output, 'client3'))

    print "-On Client 4-"
    output = extractOutputSuccess("LIST", logfile_path+"_1414")
    parseOutput(output, lambda: parseLIST(output, 'client4'))

    #SEND
    print
    print '\033[33m'+"SEND ..."+'\033[0m',
    sys.stdout.flush()
    s_or_c = 's'
    port = 4242
    expect_command = "expect -f send_server.exp "+args.path[0]+" "+s_or_c+" "+str(port)
    server = subprocess.Popen(expect_command, shell=True, stdout=open(os.devnull, 'w'), stderr=subprocess.STDOUT)
    s_or_c = 'c'
    port = 1111
    expect_command = "expect -f send_client.exp "+args.path[0]+" "+s_or_c+" "+str(port)
    client = subprocess.Popen(expect_command, shell=True, stdout=open(os.devnull, 'w'), stderr=subprocess.STDOUT)
    time.sleep(8)

    os.system('kill -9 '+str(client.pid))

    print
    print "I got the following output:"
    print "-On Server-"
    output = extractOutputSuccess("RELAYED", logfile_path+"_4242")
    parseOutput(output, lambda: parseRELAYED(output))
    print
    print "-On Client-"
    output = extractOutputSuccess("RECEIVED", logfile_path+"_1111")
    parseOutput(output, lambda: parseRECEIVED(output))
    print

    #BROADCAST
    print
    print '\033[33m'+"BROADCAST ..."+'\033[0m',
    sys.stdout.flush()
    s_or_c = 's'
    port = 4242
    expect_command = "expect -f broadcast_server.exp "+args.path[0]+" "+s_or_c+" "+str(port)
    server = subprocess.Popen(expect_command, shell=True, stdout=open(os.devnull, 'w'), stderr=subprocess.STDOUT)
    s_or_c = 'c'
    port = 1111
    expect_command = "expect -f broadcast_client_1.exp "+args.path[0]+" "+s_or_c+" "+str(port)
    client_1 = subprocess.Popen(expect_command, shell=True, stdout=open(os.devnull, 'w'), stderr=subprocess.STDOUT)
    port = 1212
    expect_command = "expect -f broadcast_client_2.exp "+args.path[0]+" "+s_or_c+" "+str(port)
    client_2 = subprocess.Popen(expect_command, shell=True, stdout=open(os.devnull, 'w'), stderr=subprocess.STDOUT)
    time.sleep(10)

    os.system('kill -9 '+str(client_1.pid))
    os.system('kill -9 '+str(client_2.pid))

    print
    print "I got the following output:"
    print "-On Server-"
    output = extractOutputSuccess("RELAYED", logfile_path+"_4242")
    parseOutput(output, lambda: parseRELAYED(output))
    print
    print "-On Client 1-"
    output = extractOutputSuccess("RECEIVED", logfile_path+"_1111")
    parseOutput(output, lambda: parseRECEIVED(output))
    print
    print "-On Client 2-"
    output = extractOutputSuccess("RECEIVED", logfile_path+"_1212")
    parseOutput(output, lambda: parseRECEIVED(output))
    print

    #STATISTICS
    print
    print '\033[33m'+"STATISTICS ..."+'\033[0m',
    sys.stdout.flush()
    s_or_c = 's'
    port = 4242
    expect_command = "expect -f statistics_server.exp "+args.path[0]+" "+s_or_c+" "+str(port)
    server = subprocess.Popen(expect_command, shell=True, stdout=open(os.devnull, 'w'), stderr=subprocess.STDOUT)
    s_or_c = 'c'
    port = 1111
    expect_command = "expect -f statistics_client_1.exp "+args.path[0]+" "+s_or_c+" "+str(port)
    client_1 = subprocess.Popen(expect_command, shell=True, stdout=open(os.devnull, 'w'), stderr=subprocess.STDOUT)
    port = 1212
    expect_command = "expect -f statistics_client_2.exp "+args.path[0]+" "+s_or_c+" "+str(port)
    client_2 = subprocess.Popen(expect_command, shell=True, stdout=open(os.devnull, 'w'), stderr=subprocess.STDOUT)
    time.sleep(15)

    os.system('kill -9 '+str(client_1.pid))
    os.system('kill -9 '+str(client_2.pid))

    print
    print "I got the following output:"
    output = extractOutputSuccess("STATISTICS", logfile_path+"_4242")
    parseOutput(output, lambda: parseSTATISTICS(output))
    print
