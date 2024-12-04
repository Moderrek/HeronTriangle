# Heron Triangle

![image](https://github.com/user-attachments/assets/47fd1bd0-f616-455a-8e7c-dd6056ac430c)

## How to Compile

### Requirements
- **CMake** (version 3.16 or higher)
- **C++ Compiler** with support for C++20
- **Git** for cloning the repository
- **OpenGL**

### Steps to Compile

1. Clone the repository with submodules:
   ```bash
   git clone --recursive https://github.com/Moderrek/HeronTriangle.git
   cd HeronTriangle
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
