#!/usr/bin/env python3
"""
Routes for the Flask app
"""

from flask import jsonify, Response, render_template
from . import app
import logging
import time
import json

@app.route("/", methods=["GET"])
def metrics():
    """Route to fetch and return system metrics as JSON every 5 seconds."""
    from . import SystemOverview
    system_overview = SystemOverview()
    
    # Fetch metrics data
    metrics_data = system_overview.get_metrics()
    if metrics_data:
        logging.info("Metrics retrieved and streamed as JSON")
    else:
        metrics_data = {
            "cpu_usage": 0,
            "used_mem_gb": 0,
            "total_mem_gb": 16,
            "disk_used_gb": 0,
            "disk_total_gb": 500,
            "network_usage_mbps": 0
        }
        logging.error("Failed to retrieve metrics; streaming error response")
    
    