#include "main.h"

/**
 * get_all_metrics - Allocates memory for the metrics structure and
 * retrieves system metrics.
 *
 * Return: Pointer to the populated metrics structure or NULL on failure.
 */
metrics_t *get_all_metrics(void)
{
    metrics_t *metrics;
    memory_stats_t *memory_stats;
    network_stats_t *network_stats;
    disk_stats_t *disk_stats;

    metrics = malloc(sizeof(metrics_t));
    if (!metrics)
    {
        printf("Failed to allocate memory for metrics structure.\n");
        return (NULL);
    }

    /* Retrieve CPU usage */
    metrics->cpu_usage = cpu_usage();
    if (metrics->cpu_usage < 0)
    {
        printf("Failed to retrieve CPU usage.\n");
        free(metrics);
        return (NULL);
    }

    /* Retrieve memory metrics */
    memory_stats = get_memory_usage();
    if (!memory_stats)
    {
        printf("Failed to retrieve memory metrics.\n");
        free(metrics);
        return (NULL);
    }
    metrics->total_mem_gb = memory_stats->total_mem_gb;
    metrics->used_mem_gb = memory_stats->used_mem_gb;
    free(memory_stats);

    /* Retrieve disk metrics */
    disk_stats = get_disk_usage();
    if (!disk_stats)
    {
        printf("Failed to retrieve disk metrics.\n");
        free(metrics);
        return (NULL);
    }
    metrics->total_disk = disk_stats->total_space;
    metrics->used_disk = disk_stats->used_space;
    metrics->free_disk = disk_stats->free_space;
    free(disk_stats);

    /* Retrieve network metrics */
    network_stats = get_network_usage();
    if (!network_stats)
    {
        printf("Failed to retrieve network metrics.\n");
        free(metrics);
        return (NULL);
    }
    metrics->download_kbps = network_stats->download_kbps;
    metrics->upload_kbps = network_stats->upload_kbps;
    free(network_stats);

    return (metrics);
}

/**
 * free_metrics - Frees the allocated memory for the metrics structure.
 * @metrics: Pointer to the metrics structure to free.
 */
void free_metrics(metrics_t *metrics)
{
    if (metrics)
        free(metrics);
}