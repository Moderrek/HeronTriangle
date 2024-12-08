#!/bin/bash
echo "Building the project in Debug mode..."
cd "$(dirname "$0")"
cd ..
if [ ! -d "CMakeFiles" ]; then
    ./scripts/linux_setup_project.sh
fi
cmake --build . --config Debug
echo "Completed building in Debug mode."
