#ifndef MAIN_H
#define MAIN_H

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

typedef struct {
    float cpu_usage;
    float total_mem_gb;
    float used_mem_gb;
    float total_disk;
    float used_disk;
    float free_disk;
    float download_kbps;
    float upload_kbps;
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

typedef struct network_stats {
    float download_kbps;
    float upload_kbps;
} network_stats_t;

/* service_stats_t* get_services_info(int *service_count); */
metrics_t* get_all_metrics() 
float cpu_usage(void);
memory_stats_t* get_memory_usage(void);
disk_stats_t* get_disk_usage(void);
network_stats_t* get_network_usage(void);

#endif /* MAIN_H */
