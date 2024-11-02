#include <stdio.h>
#include <string.h>

const char* check_service_exists(const char* service_name) {
    char command[128];
    char result[128];
    FILE *fp;

    snprintf(command, sizeof(command), "systemctl is-active %s", service_name);
    fp = popen(command, "r");
    if (fp == NULL) {
        return "Error: could not execute command.";
    }

    fgets(result, sizeof(result), fp);
    pclose(fp);

    if (strncmp(result, "active", 6) == 0) {
        return "Service is running.";
    } else {
        return "Service is not running or does not exist.";
    }
}

