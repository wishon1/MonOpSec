#/urs/bin/env python3

import ctypes

# Load the shared library
system_info = ctypes.CDLL('./libsystem_info.so')

# Call the C function directly from Python
system_info.get_system_services()

