#include "system.h"

/**
 * get_memory_usage - Get the system memory info and calculate memory usage
 * 
 * This function reads from /proc/meminfo and calculates the memory usage
 * in GB.
 * Return: 0 on success, -1 on failure.
 */
int get_memory_usage(void)
{
    FILE *file_ptr;
    char buffer[256];
    char *fget_ptr;
    int scanf_one, scanf_two, scanf_three;
    unsigned long total_mem = 0, free_mem = 0, available_mem = 0, mem_used = 0;

    file_ptr = fopen("/proc/meminfo", "r");
    if (file_ptr == NULL)
    {
        perror("Error opening /proc/meminfo");
        return (-1);  /* Return -1 on failure */
    }

    /* Loop to process each line in /proc/meminfo */
    while (1)
    {
        fget_ptr = fgets(buffer, sizeof(buffer), file_ptr);
        if (fget_ptr == NULL)
            break;

        /* Extract total memory */
        scanf_one = sscanf(buffer, "MemTotal: %lu", &total_mem);
        if (scanf_one == 1)
            continue;

        /* Extract available memory */
        scanf_two = sscanf(buffer, "MemAvailable: %lu", &available_mem);
        if (scanf_two == 1)
            continue;

        /* Extract free memory (used as fallback if MemAvailable is not present) */
        scanf_three = sscanf(buffer, "MemFree: %lu", &free_mem);
        if (scanf_three == 1)
            continue;
    }

    fclose(file_ptr);

    /* Use MemAvailable if present; otherwise, use MemFree */
    if (available_mem > 0)
    {
        mem_used = total_mem - available_mem;
    }
    else
    {
        mem_used = total_mem - free_mem;
    }

    /* Convert kilobytes to gigabytes */
    float total_mem_gb = total_mem / (1024.0 * 1024.0);  /* KB to GB */
    float used_mem_gb = mem_used / (1024.0 * 1024.0);    /* KB to GB */

    /* Print memory usage in the format "X GB used / Y GB total" */
    printf("Memory Usage: %.2f GB used / %.2f GB total\n", used_mem_gb, total_mem_gb);
    instead of printing send the %.2f GB used / %.2f GB total\n", used_mem_gb, total_mem_gb to the send were it will be send to the pyhhon program to be used in the dashboard

    return to the main function;  /* Return 0 on success */
}
