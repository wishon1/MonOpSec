document.addEventListener("DOMContentLoaded", function () {
    fetchMetrics();

    /* metrics every 5 seconds */
    setInterval(fetchMetrics, 5000);
});

function fetchMetrics() {
    fetch('/system-overview')
        .then(response => response.json())
        .then(data => {
            const metrics = data.metrics;
            updateMetrics(metrics);
        })
        .catch(error => console.error("Error fetching metrics:", error));
}

function updateMetrics(metrics) {
    document.getElementById("cpuUsage").textContent = `CPU Usage: ${metrics.cpu_usage}%`;
    document.getElementById("memoryUsage").textContent = `Memory Usage: ${metrics.memory_usage}%`;
    document.getElementById("diskUsage").textContent = `Disk Usage: ${metrics.disk_usage}%`;
}

