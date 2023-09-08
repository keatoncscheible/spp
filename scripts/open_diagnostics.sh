#!/bin/bash

# Check if gnome-terminal is available
if command -v gnome-terminal &>/dev/null; then
    # Open a new terminal window and run the 'tail -f' command
    gnome-terminal -- bash -c "tail -f diagnostics/diagnostics_log.txt; exec bash"
else
    echo "gnome-terminal is not available on your system."
fi
