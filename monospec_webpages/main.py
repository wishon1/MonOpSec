#!/usr/bin/env python3
"""
Main entry point to run the Flask application
"""

from app import app

if __name__ == "__main__":
    app.run(debug=True)

