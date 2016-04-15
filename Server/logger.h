#ifndef LOGGER_H_
#define LOGGER_H_

#define FILEPATH_LEN 256

extern char LOGFILE[FILEPATH_LEN];

extern int ret_print, ret_log;

void cse4589_init_log(char* port);
void cse4589_print_and_log(const char* format, ...);

#endif
