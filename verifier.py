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
