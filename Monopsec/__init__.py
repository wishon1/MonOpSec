#!/usr/bin/env python3
"""
Module for app initialization
"""

import logging
from flask import Flask
from .config import Config

# Initialize the Flask application
app = Flask(__name__)
app.config.from_object(Config)

# Configure logging based on config
logging.basicConfig(
    level=app.config['LOG_LEVEL'],
    format="%(asctime)s - %(levelname)s - %(message)s"
)

# Import routes to register them with the app
from . import route
from .Models.system_overview import SystemOverview
