#!/usr/bin/env python3
import ctypes
import logging
import time
import os

class SystemOverview:
    def __init__(self, library_path=None):
        """
        Initialize SystemOverview with a path to the shared library.
        """
        try:
            if library_path is None:
                library_path = os.path.join(os.path.dirname(__file__), "lib_system_overview.so")

            if not os.path.exists(library_path):
                raise FileNotFoundError(f"Library not found at {library_path}")

            self.lib = ctypes.CDLL(library_path)
            logging.debug(f"Successfully loaded library from {library_path}")

            class Metrics(ctypes.Structure):
                _fields_ = [
                    ('cpu_usage', ctypes.c_float),
                    ('total_mem_gb', ctypes.c_float),
                    ('used_mem_gb', ctypes.c_float),
                    ('total_disk', ctypes.c_float),
                    ('used_disk', ctypes.c_float),
                    ('free_disk', ctypes.c_float),
                    ('download_kbps', ctypes.c_float),
                    ('upload_kbps', ctypes.c_float),
                ]

            self.lib.get_all_metrics.restype = ctypes.POINTER(Metrics)
            self.lib.free.restype = None
            self._metrics_type = Metrics
            self.cache = None
            self.last_cache_time = 0
            self.refresh_interval = 5

        except Exception as e:
            logging.error(f"Error loading shared library: {e}")
            self.lib = None

    def format_sig_fig(self, value):
        """Format the value to 2 significant figures as a float."""
        return float(f"{value:.2g}")

    def _fetch_metrics(self):
        """Fetch metrics from the C library, returning as a dictionary."""
        if not self.lib:
            logging.error("Shared library is not loaded.")
            return None

        try:
            metrics_ptr = self.lib.get_all_metrics()
            if not metrics_ptr:
                logging.error("Failed to retrieve metrics pointer.")
                return None

            metrics = metrics_ptr.contents

            metrics_dict = {
                "cpu": {"usage": self.format_sig_fig(metrics.cpu_usage)},
                "memory": {
                    "total_memory": self.format_sig_fig(metrics.total_mem_gb),
                    "used_memory": self.format_sig_fig(metrics.used_mem_gb),
                },
                "disk": {
                    "total_space": self.format_sig_fig(metrics.total_disk),
                    "used_space": self.format_sig_fig(metrics.used_disk),
                    "free_space": self.format_sig_fig(metrics.free_disk),
                },
                "network": {
                    "download_kbps": self.format_sig_fig(metrics.download_kbps),
                    "upload_kbps": self.format_sig_fig(metrics.upload_kbps),
                },
            }

            self.lib.free(metrics_ptr)
            return metrics_dict

        except Exception as e:
            logging.error(f"Error retrieving metrics: {e}")
            return None

    def get_metrics(self):
        """Return cached or newly fetched system metrics."""
        current_time = time.time()
        if not self.cache or (current_time - self.last_cache_time > self.refresh_interval):
            self.cache = self._fetch_metrics()
            self.last_cache_time = current_time

        return self.cache
