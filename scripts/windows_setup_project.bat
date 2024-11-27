@echo off
REM Script to configure the project using CMake
echo Configuring the project...
cd /d "%~dp0"
cd ..
cmake -S . -B .
echo Project configured successfully.
pause