/**
 * @logger
 * @author  Swetank Kumar Saha <swetankk@buffalo.edu>
 * @version 1.0
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
 *
 * @section DESCRIPTION
 *
 * Contains logging functions to be used by CSE489/589 students.
 */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <unistd.h>

#include "global.h"
#include "logger.h"

char LOGFILE[FILEPATH_LEN];
void cse4589_init_log(char* port)
{
	/*Get hostname and build file paths*/
	FILE* fp;
	fp = popen("echo $HOSTNAME | tr '.' '\n' | sed -n 1p", "r"); //Gets the local hostname (without the domain name part)
	if (fp == NULL) {
    	printf("Oops! Failed to get hostname. Contact the course staff!\n" );
		exit(1);
	}

	char* hostname = (char*) malloc(HOSTNAME_LEN*sizeof(char));
	bzero(hostname, HOSTNAME_LEN);
	fscanf(fp, "%s[^\n]", hostname);

	/* Get executable's path*/
	char* exec_path = (char*) malloc(PATH_LEN*sizeof(char));
	bzero(exec_path, PATH_LEN);
  	if (readlink("/proc/self/exe", exec_path, PATH_LEN) == -1) {
    	printf("Oops! Failed to get executable path. Contact the course staff!\n" );
		exit(1);
	}

	char* exec_dir = (char*) malloc(PATH_LEN*sizeof(char));
	bzero(exec_dir, PATH_LEN);
	memcpy(exec_dir, exec_path, strlen(exec_path)-12);
	free(exec_path);

	bzero(LOGFILE, FILEPATH_LEN);
	sprintf(LOGFILE, "%s/logs/assignment_log_%s_%s", exec_dir, hostname, port);

	/*Clean up*/
	free(exec_dir);
	free(hostname);
	fclose(fp);
}

/**
 * Writes the C string passed in format to both STDOUT and LOGFILE.
 * Like printf, replaces the format specifiers with the given arguments.
 * Return code is written to ret_print (for printing to STDOUT) 
 * and ret_log (for the LOGFILE printing).
 *
 * ret_print either contains the number of characters printed OR a negative value 
 * indicating the error code.
 * ret_log either contains the number of characters logged OR a negative value 
 * indicating the error code. error code -100 indicates unable to open LOGFILE.
 *
 * @param  format Format string to be printed
 * @param  ... Variable number of arguments to replace format specifiers
 */
int ret_print, ret_log;
void cse4589_print_and_log(const char* format, ...)
{
	va_list args_pointer;

	/* Print to STDOUT */
	va_start(args_pointer, format);
   	ret_print = vprintf(format, args_pointer);

	/* Write to LOG File */
	FILE* fp;
	if((fp = fopen(LOGFILE, "a")) == NULL){
		ret_log = -100;
		/* clean up before exit */
		va_end(args_pointer);
	}

	va_start(args_pointer, format);
	ret_log = vfprintf(fp, format, args_pointer);

	/* clean up */
	fclose(fp);
    va_end(args_pointer);
}
