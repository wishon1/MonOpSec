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
    float cpu_percentage;
    memory_stats_t *mem_stats;
    disk_stats_t *disk_stats;
    network_stats_t *net_stats;

    curl_global_init(CURL_GLOBAL_ALL);

    while (1) {
        cpu_percentage = read_cpu_usage();
        if (cpu_percentage < 0) {
            fprintf(stderr, "Error retrieving CPU usage\n");
            sleep(5);
            continue;
        }

        mem_stats = get_memory_usage();
        if (!mem_stats) {
            fprintf(stderr, "Error retrieving memory usage\n");
            sleep(5);
            continue;
        }

        disk_stats = get_disk_usage();
        if (!disk_stats) {
            fprintf(stderr, "Error retrieving disk usage\n");
            free(mem_stats);
            sleep(5);
            continue;
        }

        net_stats = get_network_usage();
        if (!net_stats) {
            fprintf(stderr, "Error retrieving network usage\n");
            free(mem_stats);
            free(disk_stats);
            sleep(5);
            continue;
        }

        send_data_to_flask(cpu_percentage, mem_stats, disk_stats, net_stats);

        /* Free dynamically allocated memory */
        free(mem_stats);
        free(disk_stats);
        free(net_stats);

        /* Send updates every 5 seconds */
        sleep(5);
    }

    curl_global_cleanup();
    return 0;
}