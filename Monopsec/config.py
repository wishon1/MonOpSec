#!/usr/bin/env python3
"""
Configuration values for the Flask app
"""

import logging

class Config:
    SECRET_KEY = 'monopsec'
    DEBUG = True
    LOG_LEVEL = logging.DEBUG
    REFRESH_INTERVAL = 5 