#!/usr/bin/python3
"""main file to test system_overview"""

from .system_overview import SystemOverview, time, logging

def main():
    # Configure logging to display debug messages
    logging.basicConfig(level=logging.DEBUG, format='%(asctime)s - %(levelname)s - %(message)s')
    
    # Initialize SystemOverview instance
    system_overview = SystemOverview()

    # Continuously print metrics at the specified interval
    while True:
        # Retrieve metrics
        metrics = system_overview.get_metrics()
        
        if metrics:
            # Print metrics in a structured format
            print("\nSystem Metrics:")
            print(f"CPU Usage: {metrics['cpu']['usage']}%")
            print(f"Memory - Total: {metrics['memory']['total_memory']} GB, Used: {metrics['memory']['used_memory']} GB")
            print(f"Disk - Total: {metrics['disk']['total_space']} GB, Used: {metrics['disk']['used_space']} GB, Free: {metrics['disk']['free_space']} GB")
            print(f"Network - Download: {metrics['network']['download_kbps']} kbps, Upload: {metrics['network']['upload_kbps']} kbps")
        else:
            print("Failed to retrieve metrics.")
        
        # Wait before fetching metrics again
        time.sleep(system_overview.refresh_interval)

if __name__ == "__main__":
    main()
