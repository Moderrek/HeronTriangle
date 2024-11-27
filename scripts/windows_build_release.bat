@echo off
REM Script to build the project in Release Mode
echo Building the project in Release mode...
cd /d "%~dp0"
cd ..
cmake --build . --config Release
echo Build completed successfully.
pause