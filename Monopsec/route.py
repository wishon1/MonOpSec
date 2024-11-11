from flask import jsonify, render_template
from . import app
import logging

@app.route("/", methods=["GET"])
def system_overview():
    """Route to render the HTML template for system overview."""
    return render_template("overview.html")

@app.route("/metrics", methods=["GET"])
def metrics():
    """Route to fetch and return system metrics as JSON."""
    from . import SystemOverview
    system_overview = SystemOverview()
    
    # Fetch metrics data
    metrics_data = system_overview.get_metrics()
    if metrics_data:
        logging.info("Metrics retrieved successfully")
    else:
        # Default values if retrieval fails
        
        metrics_data = {
            "cpu": {"usage": 0, "history": [0] * 20},
            "memory": {"used_memory": 0, "total_memory": 0, "history": [0] * 20},
            "disk": {"used_space": 0, "total_space": 0},
            "network": {"download_kbps": 0}
        }
        logging.error("Failed to retrieve metrics; using default values")
    
    # Return metrics data as JSON
    return jsonify(metrics_data)
