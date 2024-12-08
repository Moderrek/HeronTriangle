@echo off
REM Script to build the project in Debug Mode
echo Building the project in Debug mode...
cd /d "%~dp0"
cd ..
cmake -DCMAKE_BUILD_TYPE=Debug .
cmake --build . --config Debug
echo Build completed successfully.
pause