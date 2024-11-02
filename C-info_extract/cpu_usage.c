#include "main.h"
/**
 * cpu_usage - Calculate the CPU usage as a percentage.
 *
 * This function reads CPU statistics from /proc/stat and calculates
 * the CPU usage by subtracting idle time from the total CPU time
 * and converting it to a percentage.
 *
 * Return: CPU usage as a percentage on success, -1 on failure.
 */
float cpu_usage(void)
{
    FILE *file_ptr;
    char buffer[256];
    char *fget_ptr;
    unsigned long long int user, nice, system, idle, total;
    float cpu_usage_percentage;
    int sscanf_ret;

    file_ptr = fopen("/proc/stat", "r");
    if (file_ptr == NULL)
    {
        perror("Error opening /proc/stat");
        return (-1);
    }

    /* Read the first line of the file (CPU statistics) */
    fget_ptr = fgets(buffer, sizeof(buffer), file_ptr);
    if (fget_ptr == NULL)
    {
        perror("Error reading from /proc/stat");
        fclose(file_ptr);
        return (-1);
    }

    /* Parse the CPU statistics from the first line */
    sscanf_ret = sscanf(buffer, "cpu %llu %llu %llu %llu", &user, &nice, &system, &idle);
    if (sscanf_ret != 4)
    {
        perror("Error parsing /proc/stat");
        fclose(file_ptr);
        return (-1);
    }
    fclose(file_ptr);

    /* Calculate the total time spent by the CPU */
    total = user + nice + system + idle;
    cpu_usage_percentage = (float)(total - idle) / (float)total * 100.0;

    return (cpu_usage_percentage);
}