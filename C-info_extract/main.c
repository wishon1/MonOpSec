#include "main.h"

/**
 * main - Retrieve OS system information and send to Python
 * 
 * This function retrieves CPU, memory, disk, and network information
 * and sends it to a Python Flask program at regular intervals.
 * Return: 0 on success, non-zero on failure.
 */

int main(void)
{
    float usage;
    disk_stats_t *disk_stats;
    memory_stats_t *memory_stats;
    network_stats_t *net_stats;

    usage = cpu_usage();
    if (usage >= 0)
    {
        printf("CPU Usage: %.2f\n", usage);
    }
    else
    {
        fprintf(stderr, "Failed to retrieve CPU usage.\n");
        return (1);
    }

    memory_stats = get_memory_usage();
    if (memory_stats)
    {
        /* Memory in Megabytes 'MB' */
        printf("Total Memory: %.2f\n", memory_stats->total_mem_gb);
        printf("Used Memory: %.2f\n", memory_stats->used_mem_gb);
        /* printf("Free Memory: %.2f\n", memory_stats->free_mem); */

        free(memory_stats);
    }
    else
    {
        fprintf(stderr, "Failed to retrieve memory usage statistics.\n");
        return (1);
    }

    disk_stats = get_disk_usage();
    if (disk_stats)
    {
        /* Disk space in Gigabytes 'GB' */
        printf("Total Disk Space: %.2f\n", disk_stats->total_space);
        printf("Used Disk Space: %.2f\n", disk_stats->used_space);
        printf("Free Disk Space: %.2f\n", disk_stats->free_space);

        free(disk_stats);
    }
    else
    {
        printf("Failed to retrieve disk usage statistics.\n");
    }

    net_stats = get_network_usage();
    if (net_stats)
    {
        /* Download and upload speed in Kilobytes per second 'KB/s' */
        printf("Download Speed: %.2f\n", net_stats->download_kbps);
        printf("Upload Speed: %.2f\n", net_stats->upload_kbps);
        
        free(net_stats);
    }
    else
    {
        printf("Failed to retrieve network usage statistics.\n");
    }

    return 0;
}
