#!/usr/bin/python3
import ctypes
import json
import time
from flask import Flask, jsonify, render_template

app = Flask(__name__)

# Load the shared C library
cpu_lib = ctypes.CDLL('./cpu_history.so')

# Define the function signature for collect_cpu_history
cpu_lib.collect_cpu_history.argtypes = [ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_int)]
cpu_lib.collect_cpu_history.restype = None

@app.route('/get_cpu_usage')
def get_cpu_usage():
    # Prepare an array to store CPU usage history
    cpu_usage_history = (ctypes.c_double * 100)()  # Array of 100 double values
    history_size = ctypes.c_int(0)

    # Call the C function to collect the CPU usage history
    cpu_lib.collect_cpu_history(cpu_usage_history, ctypes.byref(history_size))

    # Convert C array to a Python list
    cpu_usage_list = [cpu_usage_history[i] for i in range(history_size.value)]

    # Return the data as JSON
    return jsonify(cpu_usage=cpu_usage_list)

if __name__ == '__main__':
    app.run(debug=True)

