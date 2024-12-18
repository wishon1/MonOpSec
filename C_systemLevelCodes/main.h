#ifndef MAIN_H
#define MAIN_H

#define MAX_HISTORY 100

#include <stdio.h>
#include <stdlib.h>
#include <sys/statvfs.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>
#include <ctype.h>
#include <stdint.h>
#include <sys/sysinfo.h>
#include <sys/types.h>
#include <time.h>

typedef struct {
    float cpu_usage;
    float total_mem_gb;
    float used_mem_gb;
    float total_disk;
    float used_disk;
    float free_disk;
    float download_kbps;
    float upload_kbps;
    CpuEntry cpu_usage_history[MAX_HISTORY];
    int history_size;
} metrics_t;

/**
 * typedef struct service_stats {
    char *name; 
    char *description;
    float cpu_usage;   
    unsigned long memory_usage;
    char *status;
} service_stats_t;

*/ 

typedef struct memory_stats {
    unsigned long total_mem;
    unsigned long available_mem;
    unsigned long used_mem;
    float total_mem_gb;
    float used_mem_gb;
} memory_stats_t;

typedef struct disk_stats {
    float total_space;
    float used_space;
    float free_space;
} disk_stats_t;

typedef struct network_stats
{
    float download_kbps;
    float upload_kbps;
} network_stats_t;

typedef struct
{
    double usage;
    time_t timestamp;
} CpuEntry;

/* service_stats_t* get_services_info(int *service_count); */
void get_cpu_usage(double *cpu_usage, int *num_entries);
void collect_cpu_history(CpuEntry *history, int *size);
metrics_t *get_all_metrics();
float cpu_usage();
memory_stats_t* get_memory_usage();
disk_stats_t* get_disk_usage();
network_stats_t* get_network_usage();

#endif /* MAIN_H */
