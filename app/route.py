#!/usr/bin/env python3
"""
Routes for the Flask app
"""

from flask import jsonify
from app import app  # Import the app instance
import subprocess
import re
import logging

def get_metrics():
    try:
        result = subprocess.run(["./main"], capture_output=True, text=True, check=True)
        output = result.stdout
        logging.debug("C program output captured successfully")
    except subprocess.CalledProcessError as e:
        logging.error(f"Error executing main program: {e}")
        return None

    # Parse output to extract metrics
    cpu_usage = re.search(r"CPU Usage:\s*([\d.]+)", output)
    total_memory = re.search(r"Total Memory:\s*([\d.]+)", output)
    used_memory = re.search(r"Used Memory:\s*([\d.]+)", output)
    free_memory = re.search(r"Free Memory:\s*([\d.]+)", output)
    total_disk = re.search(r"Total Disk Space:\s*([\d.]+)", output)
    used_disk = re.search(r"Used Disk Space:\s*([\d.]+)", output)
    free_disk = re.search(r"Free Disk Space:\s*([\d.]+)", output)
    download_speed = re.search(r"Download Speed:\s*([\d.]+)", output)
    upload_speed = re.search(r"Upload Speed:\s*([\d.]+)", output)

    metrics = {
        "cpu": {"usage": float(cpu_usage.group(1)) if cpu_usage else None},
        "memory": {
            "total_memory": float(total_memory.group(1)) if total_memory else None,
            "used_memory": float(used_memory.group(1)) if used_memory else None,
            "free_memory": float(free_memory.group(1)) if free_memory else None,
        },
        "disk": {
            "total_space": float(total_disk.group(1)) if total_disk else None,
            "used_space": float(used_disk.group(1)) if used_disk else None,
            "free_space": float(free_disk.group(1)) if free_disk else None,
        },
        "network": {
            "download_kbps": float(download_speed.group(1)) if download_speed else None,
            "upload_kbps": float(upload_speed.group(1)) if upload_speed else None,
        },
    }

    logging.debug("Metrics parsed successfully")
    return metrics

@app.route("/metrics", methods=["GET"])
def metrics():
    data = get_metrics()
    if data:
        logging.info("Metrics retrieved and returned as JSON")
        return jsonify(data)
    else:
        logging.error("Failed to retrieve metrics; returning error response")
        return jsonify({"error": "Failed to retrieve metrics"}), 500

