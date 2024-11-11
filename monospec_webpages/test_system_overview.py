#!/usr/bin/env python3
"""
Script to print system metrics from the SystemOverview class.
"""

from python3_Module import SystemOverview
def main():
    # Initialize SystemOverview instance
    system_metrics = SystemOverview()

    # Retrieve and print system metrics
    metrics = system_metrics.get_metrics()
    if metrics:
        print("System Metrics:")
        print("CPU Usage:", metrics["cpu"]["usage"], "%")
        print("Memory Total:", metrics["memory"]["total_memory"], "bytes")
        print("Memory Used:", metrics["memory"]["used_memory"], "bytes")
        print("Disk Total:", metrics["disk"]["total_space"], "GB")
        print("Disk Used:", metrics["disk"]["used_space"], "GB")
        print("Disk Free:", metrics["disk"]["free_space"], "GB")
        print("Network Download Speed:", metrics["network"]["download_kbps"], "kbps")
        print("Network Upload Speed:", metrics["network"]["upload_kbps"], "kbps")
    else:
        print("Failed to retrieve metrics.")

if __name__ == "__main__":
    main()

