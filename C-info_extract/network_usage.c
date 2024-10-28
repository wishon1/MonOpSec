#include "main.h"

/**
 * get_network_usage - Retrieves network usage statistics.
 *
 * This function reads the /proc/net/dev file to gather
 * the number of bytes received (rx) and transmitted (tx) by
 * the network interfaces. It skips the header lines and sums
 * up the total received and transmitted bytes for all interfaces.
 *
 * Return: A pointer to network_stats_t structure containing
 *         network usage in KB/s format for download and upload.
 *         Returns NULL if an error occurs.
 */
network_stats_t *get_network_usage(void) {
    FILE *file_ptr = fopen("/proc/net/dev", "r");
    char buffer[256];
    unsigned long temp_rx_bytes, temp_tx_bytes, rx_bytes = 0, tx_bytes = 0;
    char *fget_output;
    int parsed;
    network_stats_t *net_stats = malloc(sizeof(network_stats_t));

    if (!net_stats) {
        perror("Memory allocation error");
        return NULL;
    }

    if (!file_ptr) {
        perror("Error opening /proc/net/dev");
        free(net_stats);
        return NULL;
    }

    /* Skip the first header line */
    fgets(buffer, sizeof(buffer), file_ptr);

    /* skip the second header line */
    fgets(buffer, sizeof(buffer), file_ptr);

    while (1) {
        fget_output = fgets(buffer, sizeof(buffer), file_ptr);
        if (fget_output == NULL)
            break;

        // Parse the line for rx and tx bytes
        parsed = sscanf(buffer, "%*s %lu %*s %*s %*s %*s %*s %*s %lu", 
                       &temp_rx_bytes, &temp_tx_bytes);
        if (parsed == 2) {
            rx_bytes += temp_rx_bytes;
            tx_bytes += temp_tx_bytes;
        } else {
            fprintf(stderr, "Error parsing line: %s\n", buffer);
        }
    }
    fclose(file_ptr);

    if (rx_bytes == 0 && tx_bytes == 0) {
        free(net_stats);
        return NULL;
    }

    net_stats->download_kbps = rx_bytes / 1024.0;
    net_stats->upload_kbps = tx_bytes / 1024.0;

    return net_stats;
}

/**
 * main - Test the get_network_usage function
 *
 * Return: Always 0 on success, non-zero on failure.
 */
int main(void) {
    network_stats_t *net_stats = get_network_usage();

    if (net_stats) {
        printf("Download Speed: %.2f KB/s\n", net_stats->download_kbps);
        printf("Upload Speed: %.2f KB/s\n", net_stats->upload_kbps);
        free(net_stats); // Free the allocated memory
    } else {
        printf("Failed to retrieve network usage statistics.\n");
    }

    return 0;
}