#!/bin/bash

# Set the directory as a variable
diagnostics_dir="diagnostics"
diagnostics_file="$diagnostics_dir/diagnostics_out.txt"

# Check if gnome-terminal is available
if command -v gnome-terminal &>/dev/null; then
    mkdir $diagnostics_dir
    touch $diagnostics_file
    gnome-terminal -- bash -c "tail -f $diagnostics_file; exec bash"
else
    echo "gnome-terminal is not available on your system."
fi
