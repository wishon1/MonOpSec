#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sysinfo.h>
#include <unistd.h>

#define MAX_SERVICES 200

/**
 * execute_command - Executes a shell command and stores the output.
 * @command: The command to execute.
 * @output: The buffer where output is stored.
 * @max_size: The maximum size of the output buffer.
 * 
 * Return: 0 on success, -1 on failure.
 */
int execute_command(const char *command, char *output, int max_size)
{
    FILE *fp;

    fp = popen(command, "r");
    if (!fp)
        return (-1);

    fgets(output, max_size, fp);
    pclose(fp);
    return (0);
}

/**
 * get_process_usage - Gets CPU and memory usage of a process.
 * @pid: The process ID.
 * @cpu_usage: Pointer to store the CPU usage.
 * @memory_usage: Pointer to store memory usage in MB.
 */
void get_process_usage(int pid, float *cpu_usage, long unsigned int *memory_usage)
{
    char stat_file[256], line[128];
    unsigned long utime, stime;
    FILE *file;
    struct sysinfo sys_info;
    float total_time;

    snprintf(stat_file, sizeof(stat_file), "/proc/%d/stat", pid);
    file = fopen(stat_file, "r");
    if (!file)
        return;

    fscanf(file, "%*d %*s %*c %*d %*d %*d %*d %*d %*u %*u %*u %*u %*u %lu %lu", &utime, &stime);
    fclose(file);

    sysinfo(&sys_info);
    total_time = (utime + stime) / sysconf(_SC_CLK_TCK);
    *cpu_usage = 100.0 * (total_time / sys_info.uptime);

    snprintf(stat_file, sizeof(stat_file), "/proc/%d/status", pid);
    file = fopen(stat_file, "r");
    if (!file)
        return;

    while (fgets(line, sizeof(line), file))
    {
        if (strncmp(line, "VmRSS:", 6) == 0)
        {
            sscanf(line, "VmRSS: %ld", memory_usage);
            *memory_usage /= 1024;
            break;
        }
    }
    fclose(file);
}

/**
 * get_services_info - Retrieves information about active services.
 * @service_count: Pointer to store the number of services.
 * 
 * Return: Pointer to an array of service_stats_t with service information.
 */
service_stats_t *get_services_info(int *service_count)
{
    char *output = malloc(1024);
    char *cmd = NULL;
    int count = 0;
    FILE *fp;
    char *fget_ptr;
    service_stats_t *services;
    int pid;

    services = malloc(MAX_SERVICES * sizeof(service_stats_t));
    if (!services)
        return (NULL);

    fp = popen("systemctl list-units --type=service --no-pager", "r");
    if (!fp)
    {
        free(services);
        free(output);
        return (NULL);
    }

    while (1)
    {
        fget_ptr = fgets(output, 1024, fp);
        if (fget_ptr == NULL)
            break;

        if (strstr(output, ".service"))
        {
            services[count].name = strdup(strtok(output, " "));
            services[count].description = strdup("Service description"); /* Placeholder */
            cmd = malloc(128);
            snprintf(cmd, 128, "systemctl show -p MainPID --value %s", services[count].name);
            execute_command(cmd, output, 1024);
            pid = atoi(output);

            if (pid > 0)
                get_process_usage(pid, &services[count].cpu_usage, &services[count].memory_usage);
            else
            {
                services[count].cpu_usage = 0.0;
                services[count].memory_usage = 0;
            }

            snprintf(cmd, 128, "systemctl is-active %s", services[count].name);
            execute_command(cmd, output, 32);
            services[count].status = strdup(output);

            count++;
            free(cmd);
            if (count >= MAX_SERVICES)
                break;
        }
    }

    pclose(fp);
    free(output);
    *service_count = count;
    return (services);
}

/**
 * display_service_info - Displays information about active and inactive services.
 * @services: Array of service_stats_t containing service data.
 * @service_count: Number of services in the array.
 */
void display_service_info(service_stats_t *services, int service_count)
{
    int i;

    printf("** Running Services **\n\n");
    for (i = 0; i < service_count; i++)
    {
        if (strcmp(services[i].status, "active\n") == 0)
        {
            printf("%s\n%s\nCPU: %.0f%%, Memory: %luMB, Status: active.\n\n",
                   services[i].name,
                   services[i].description,
                   services[i].cpu_usage,
                   services[i].memory_usage);
        }
    }

    printf("** Inactive Services **\n\n");
    for (i = 0; i < service_count; i++)
    {
        if (strcmp(services[i].status, "inactive\n") == 0)
        {
            printf("%s\n%s\nCPU: %.0f%%, Memory: %luMB, Status: inactive.\n\n",
                   services[i].name,
                   services[i].description,
                   services[i].cpu_usage,
                   services[i].memory_usage);
        }
    }

    for (i = 0; i < service_count; i++)
    {
        free(services[i].name);
        free(services[i].description);
        free(services[i].status);
    }
}

/**
 * main - Entry point for service monitoring program.
 * 
 * Return: 0 on success.
 */
int main(void)
{
    int service_count;
    service_stats_t *services = get_services_info(&service_count);

    if (services)
    {
        display_service_info(services, service_count);
        free(services);
    }
    return (0);
}
