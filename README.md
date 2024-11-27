# Heron Triangle

## How to Compile

### Requirements
- **CMake** (version 3.16 or higher)
- **C++ Compiler** with support for C++20
- **Git** for cloning the repository
- **OpenGL**

### Steps to Compile

1. Clone the repository with submodules:
   ```bash
   git clone --recursive https://github.com/Moderrek/SolarSystem.git
   cd SolarSystem
   ```

3. Configure the project using **CMake**:
   ```bash
   cmake -S . -B .
   ```

4. Build the project:
   ```bash
   cmake --build . --config Debug
   ```

5. Run the game:
   * On **Windows**: Open executable in the `build/bin` or `build/bin` directory.
