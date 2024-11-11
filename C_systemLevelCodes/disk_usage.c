#include "main.h"

/**
 * get_disk_usage - Retrieves disk usage statistics.
 *
 * This function reads the file system statistics to calculate
 * the total, free, and used disk space in gigabytes (GB).
 * It returns a dynamically allocated disk_stats structure 
 * containing the disk usage information.
 *
 * Return: A pointer to disk_stats_t structure containing the disk
 *         usage information. Returns NULL if an error occurs.
 */
disk_stats_t *get_disk_usage(void)
{
    struct statvfs stat;
    disk_stats_t *disk_stats;

    disk_stats = malloc(sizeof(disk_stats_t));
    if (!disk_stats)
    {
        perror("Memory allocation error");
        return (NULL);
    }

    if (statvfs("/", &stat) != 0)
    {
        perror("Error getting disk usage");
        free(disk_stats);
        return (NULL);
    }

    /* Calculate total and used space in GB */
    disk_stats->total_space = (stat.f_blocks * stat.f_frsize) /
                              (1024.0 * 1024.0 * 1024.0);
    disk_stats->free_space = (stat.f_bfree * stat.f_frsize) /
                             (1024.0 * 1024.0 * 1024.0);
    disk_stats->used_space = disk_stats->total_space - disk_stats->free_space;

    return (disk_stats);
}