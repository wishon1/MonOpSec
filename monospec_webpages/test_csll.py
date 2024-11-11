from flask import Flask, jsonify, render_template
import subprocess

app = Flask(__name__)

def get_cpu_usage():
    """Run the C executable to get CPU usage and return it as a float."""
    try:
        # Run the C program and capture its output
        result = subprocess.run(['./cpu_usage'], capture_output=True, text=True)
        # Parse the output to get the CPU usage as a float
        usage = float(result.stdout.strip().split()[-1].replace('%', ''))
        return usage
    except Exception as e:
        print(f"Error retrieving CPU usage: {e}")
        return None

@app.route('/')
def index():
    """Render the main page."""
    return render_template('index.html')

@app.route('/api/cpu_usage')
def cpu_usage_api():
    """API endpoint to get CPU usage."""
    usage = get_cpu_usage()
    if usage is not None:
        return jsonify({"cpu_usage": usage})
    return jsonify({"error": "Unable to retrieve CPU usage"}), 500

if __name__ == '__main__':
    app.run(debug=True)
