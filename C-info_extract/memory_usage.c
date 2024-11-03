#include "main.h"

/**
 * get_memory_usage - Get the system memory info and calculate memory usage.
 *
 * This function reads from /proc/meminfo, calculates memory usage in GB,
 * and returns the memory usage data in a memory_stats_t structure.
 *
 * Return: Pointer to memory_stats_t structure on success, NULL on failure.
 */
memory_stats_t *get_memory_usage(void)
{
    FILE *file_ptr;
    char buffer[256];
    char *fget_ptr;
    unsigned long total_mem = 0, free_mem = 0, available_mem = 0;
    memory_stats_t *stats;

    stats = calloc(1, sizeof(memory_stats_t));
    if (!stats)
    {
        perror("Memory allocation failed");
        return (NULL);
    }

    file_ptr = fopen("/proc/meminfo", "r");
    if (file_ptr == NULL)
    {
        perror("Error opening /proc/meminfo");
        free(stats);
        return (NULL);
    }

    while (1)
    {
        fget_ptr = fgets(buffer, sizeof(buffer), file_ptr);
        if (fget_ptr == NULL)
            break;

        /* Extract total memory */
        if (sscanf(buffer, "MemTotal: %lu", &total_mem) == 1)
            continue;

        /* Extract available memory */
        if (sscanf(buffer, "MemAvailable: %lu", &available_mem) == 1)
            continue;

        /* Extract free memory (used as fallback if MemAvailable is not present) */
        if (sscanf(buffer, "MemFree: %lu", &free_mem) == 1)
            continue;
    }
    fclose(file_ptr);

    if (available_mem > 0)
        stats->used_mem = total_mem - available_mem;
    else
        stats->used_mem = total_mem - free_mem;

    stats->total_mem = total_mem;
    stats->available_mem = available_mem;

    /* Convert kilobytes to gigabytes */
    stats->total_mem_gb = total_mem / (1024.0 * 1024.0);
    stats->used_mem_gb = stats->used_mem / (1024.0 * 1024.0);

    return (stats);
}
