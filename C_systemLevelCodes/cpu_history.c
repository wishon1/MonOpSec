#include "main.h"
#define MAX_ENTRIES 100


/**
 * 
 */
void get_cpu_usage(CpuEntry *entry, int *num_entries)
{
    FILE *fp;
    char buffer[256];
    double user, nice, system, idle, iowait, irq, softriq, steal;
    double total, idle_time, total_diff, idle_diff;
    static double last_total, last_idle;
    char *fgets_val;

    last_total = 0.0;
    last_idle = 0.0;

    fp = fopen("/proc/stat", "r");
    if (fp == NULL)
    {
        perror("Failed to open /proc/stat");
        return;
    }

    /* Read the first line of /proc/stat for cpu uasge data */
    fgets_val = fgets(buffer, sizeof(buffer), fp);
    if (fgets_val == NULL)
    {
        perror("Failed to read from the file stream");
        fclose(fp);
    }
    else
    {
        fclose(fp);
        /* parse the first 8 values as double */
        sscanf(buffer, "cpu %lf %lf %lf %lf %lf %lf %lf %lf", &user, &nice, &idle,\
            &iowait, &irq, &softriq, &steal);

        /* calculate total and idle time */
        total = user + nice + system + idle + iowait + irq + softriq + steal;
        idle_time = idle + iowait;

        /* calculate the difference from the last reading */
        total_diff = total - last_total;
        idle_diff = idle_time - last_idle;

        /* store the CPU usage percentage */
        if (total_diff != 0)
        {
            entry->usage = 100.0 * (total_diff - idle_diff) / total_diff;    
        }

        /* capture the current time for cpu percentage entry */
        entry->timestamp = time(NULL);

        /* Update the last value for the next read */
        last_total = total;
        last_idle = idle_time;

        /* Track the number of entries( limited to MAX_ENtries)*/
         *num_entries = (*num_entries < MAX_ENTRIES) ? *num_entries + 1 : MAX_ENTRIES;
    }
}

/**
 * 
 */
void collect_cpu_history(CpuEntry *_cpu_history, int *history_size)
{
    int num_entries;
    
    num_entries = 0;
    /* collect the cpu_usage history */
    while (num_entries < MAX_ENTRIES)
    {
        get_cpu_usage(&_cpu_history[num_entries], &num_entries);
        usleep(1000000);
    }

    *history_size = num_entries;
}