#ifndef BACKGROUND_PROCESSES_H
#define BACKGROUND_PROCESSES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/stat.h> // Add this for stat function and structure

#define MAX_PATH 1024
#define MAX_CMDLINE 4096
#define UPDATE_INTERVAL_USEC 1000000 // 1 second in microseconds

// Function prototypes
int is_numeric(const char *str);
void get_process_name(const char *pid, char *name, size_t name_size);
double get_cpu_usage(const char *pid);
long get_memory_usage(const char *pid);
void extract_background_processes();
void setup_timer();
void timer_handler(int signum);
void setup_signal_handler();
void signal_handler(int signum);

// Global variables for signal handling
extern volatile sig_atomic_t timer_flag;
extern volatile sig_atomic_t keep_running;

#endif // BACKGROUND_PROCESSES_H
