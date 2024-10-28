#include "main.h"
/**
 * 
 */
static size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp)
{
    return size * nmemb;  // Discard response data
}

void send_data_to_flask(float cpu, memory_stats_t *memory, disk_stats_t *disk, network_stats_t *network) 
{
    CURL *curl;
    CURLcode res;
    char json_data[1024];
    struct curl_slist *headers = NULL;

    // Format JSON data
    snprintf(json_data, sizeof(json_data),
             "{\"cpu_usage\": %.2f,"
             "\"memory\": {\"total_gb\": %.2f, \"used_gb\": %.2f},"
             "\"disk\": {\"total_gb\": %.2f, \"used_gb\": %.2f},"
             "\"network\": {\"download_kbps\": %.2f, \"upload_kbps\": %.2f}}",
             cpu,
             memory->total_mem_gb, memory->used_mem_gb,
             disk->total_space, disk->used_space,
             network->download_kbps, network->upload_kbps);

    curl = curl_easy_init();
    if (curl) {
        headers = curl_slist_append(headers, "Content-Type: application/json");

        curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:5000/api/metrics");
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_data);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            fprintf(stderr, "Failed to send data: %s\n", curl_easy_strerror(res));
        }

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }
}