#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void get_system_services() {
    FILE *fp;

    printf("Service Name, Status, CPU Usage, Memory Usage\n");

    // Use 'systemctl list-units --type=service --no-pager' to list all services
    fp = popen("systemctl list-units --type=service --no-pager", "r");
    if (fp == NULL) {
        perror("Failed to run command");
        exit(1);
    }

    // Read the output a line at a time
    while (getline(&buffer, &buffer_size, fp) != -1) {
        char *service_name = (char *)malloc(100 * sizeof(char));
        char *service_status = (char *)malloc(100 * sizeof(char));
        
        if (service_name == NULL || service_status == NULL) {
            perror("Failed to allocate memory for service details");
            free(buffer);
            exit(1);
        }

        // Skip the header line (if any) and ensure we are working with ".service" units
        if (strstr(buffer, "UNIT") != NULL || strstr(buffer, ".service") == NULL) {
            free(service_name);
            free(service_status);
            continue;
        }

        // Parse service name and status
        sscanf(buffer, "%99s %*s %*s %99s", service_name, service_status);

        // Process only services that are running or exited
        if (strcmp(service_status, "running") == 0 || strcmp(service_status, "exited") == 0) {
            char *cmd = (char *)malloc(256 * sizeof(char));
            if (cmd == NULL) {
                perror("Failed to allocate memory for command");
                free(service_name);
                free(service_status);
                free(buffer);
                exit(1);
            }

            snprintf(cmd, 256, "ps -C %s -o %%cpu,%%mem --no-headers", service_name);
            FILE *cpu_mem_fp = popen(cmd, "r");

            if (cpu_mem_fp != NULL) {
                double cpu_usage = 0, mem_usage = 0;

                // Check if we get valid CPU and memory usage
                if (fscanf(cpu_mem_fp, "%lf %lf", &cpu_usage, &mem_usage) == 2) {
                    printf("%s, %s, %.2f, %.2f\n", service_name, service_status, cpu_usage, mem_usage);
                } else {
                    printf("%s, %s, N/A, N/A\n", service_name, service_status);  // No matching process for this service
                }

                pclose(cpu_mem_fp);
            } else {
                printf("Failed to run ps for %s\n", service_name);
            }

            free(cmd);
        }

        free(service_name);
        free(service_status);
    }

    // Free dynamically allocated memory
    free(buffer);
    pclose(fp);
}

int main() {
    get_system_services();
    return 0;
}

