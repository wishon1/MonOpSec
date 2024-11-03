#include "main.h"
/**
*/
int main(void)
{
    metrics_t *metrics;
    
    metrics = get_all_metrics();

    if (metrics != NULL)
    {
        printf("Total Memory: %.2f GB\n", metrics->total_mem_gb);
        printf("Used Memory: %.2f GB\n", metrics->used_mem_gb);

        printf("Total disk: %.2f GB\n", metrics->total_disk);
        printf("used disk: %.2f GB\n", metrics->used_disk);
        printf("free disk: %.2f GB\n", metrics->free_disk);

        printf("CPU usage: %.2f\n", metrics->cpu_usage);

    }
    else
        printf("failed to get metrics");
    
    return (0);
}
