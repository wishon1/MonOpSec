#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/statvfs.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

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

float cpu_usage(void);
memory_stats_t* get_memory_usage(void);
disk_stats_t* get_disk_usage(void);
network_stats_t* get_network_usage(void);
void send_data_to_flask(float cpu, memory_stats_t *memory, disk_stats_t *disk, network_stats_t *network);

#endif /* MAIN_H */