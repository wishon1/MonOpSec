#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_ENTRIES 100

// Function to fetch CPU usage
void get_cpu_usage(double *cpu_usage, int *num_entries) {
    FILE *fp;
    char buffer[256];
    double user, nice, system, idle, iowait, irq, softirq, steal;
    double total, idle_time;
    static double last_total = 0.0, last_idle = 0.0;
    double total_diff, idle_diff;

    fp = fopen("/proc/stat", "r");
    if (fp == NULL) {
        perror("Failed to open /proc/stat");
        return;
    }

    // Read the first line of /proc/stat for CPU usage data
    fgets(buffer, sizeof(buffer), fp);
    fclose(fp);

    // Parse the values from the line
    sscanf(buffer, "cpu %lf %lf %lf %lf %lf %lf %lf %lf %lf", 
           &user, &nice, &system, &idle, &iowait, &irq, &softirq, &steal);

    // Calculate total and idle time
    total = user + nice + system + idle + iowait + irq + softirq + steal;
    idle_time = idle + iowait;

    // Calculate the differences from the last reading
    total_diff = total - last_total;
    idle_diff = idle_time - last_idle;

    // Store the CPU usage percentage
    if (total_diff != 0) {
        *cpu_usage = 100.0 * (total_diff - idle_diff) / total_diff;
    }

    // Update the last values for the next read
    last_total = total;
    last_idle = idle_time;

    // Track the number of entries (limited to MAX_ENTRIES)
    *num_entries = (*num_entries < MAX_ENTRIES) ? *num_entries + 1 : MAX_ENTRIES;
}

void collect_cpu_history(double *cpu_usage_history, int *history_size) {
    double cpu_usage;
    int num_entries = 0;

    // Collect CPU usage history
    while (num_entries < MAX_ENTRIES) {
        get_cpu_usage(&cpu_usage, &num_entries);
        cpu_usage_history[num_entries - 1] = cpu_usage;
        usleep(1000000);  // 1 second delay
    }

    *history_size = num_entries;
}

