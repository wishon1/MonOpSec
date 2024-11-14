#include "main.h"

#define MAX_ENTRIES 100

int main() {
    double cpu_usage_history[MAX_ENTRIES];
    int history_size = 0;

    // Collect CPU usage history
    collect_cpu_history(cpu_usage_history, &history_size);

    // Print the CPU usage history
    printf("CPU Usage History:\n");
    for (int i = 0; i < history_size; i++) {
        printf("Entry %d: %.2f%%\n", i + 1, cpu_usage_history[i]);
    }

    return 0;
}

