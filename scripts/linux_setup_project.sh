#!/bin/bash
echo "Configuring the project..."
cd "$(dirname "$0")"
cd ..
cmake -S . -B .
echo "Project configured successfully!"