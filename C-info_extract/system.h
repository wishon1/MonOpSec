#ifndef MAIN_H
#define MAIN_H

#define _GNU_SOURCE

/* headers files and libraries used*/
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

/* All functions used */
extern void get_cpu_usage();
extern int get_memory_usage();
extern void get_disk_usage();
extern void get_services_list();

#endif
