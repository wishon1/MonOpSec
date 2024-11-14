#!/usr/bin/python3
"""
route.py
"""
from flask import Flask, jsonify
import share

app = Flask(__name__)

@app.route('/cpu_usage')
def cpu_usage():
    return share.get_cpu_usage()

if __name__ == '__main__':
    app.run(debug=True)

