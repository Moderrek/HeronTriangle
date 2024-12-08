#!/bin/bash
echo "Building the project in Release mode..."
cd "$(dirname "$0")"
cd ..
if [ ! -d "CMakeFiles" ]; then
    ./scripts/linux_setup_project.sh
fi
cmake --build . --config Release
echo "Completed building in Release mode."
