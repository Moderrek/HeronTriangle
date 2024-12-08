# Heron Triangle

![Linux](https://img.shields.io/badge/Platform-Linux-blue?logo=linux)
![Windows](https://img.shields.io/badge/Platform-Windows-blue?logo=windows)
![OpenGL](https://img.shields.io/badge/Graphics-OpenGL-green?logo=opengl)
![MIT License](https://img.shields.io/badge/License-MIT-yellow?logo=opensourceinitiative)


![image](https://github.com/user-attachments/assets/47fd1bd0-f616-455a-8e7c-dd6056ac430c)

## Table of Contents
- [Heron Triangle](#heron-triangle)
  - [Table of Contents](#table-of-contents)
  - [Features](#features)
  - [How to Compile](#how-to-compile)
    - [Requirements](#requirements)
    - [Steps to Compile](#steps-to-compile)
  - [Notes](#notes)
  - [License](#license)
  - [Contributing](#contributing)
  - [Authors](#authors)

## Features
* Cross-platform support: Linux & Windows
* Real-time rendering with OpenGL 3.3+
* Lightweight and optimized for performance
* MIT License: Open for contributions

## How to Compile

### Requirements
- **CMake** (version 3.16 or higher)
- **C++ Compiler** with support for C++20
- **Git** for cloning the repository
- **OpenGL 3.3+**

### Steps to Compile

1. Clone the repository with submodules:
   ```bash
   git clone --recursive https://github.com/Moderrek/HeronTriangle.git
   cd HeronTriangle
   ```

3. Configure the project using **CMake**:  
   * For **Windows**
     ```bash
     ./scripts/windows_setup_project.bat
     ```
   * For **Linux**
     ```bash
     ./scripts/linux_setup_project.sh
     ```

4. Build the project:  
   * For **Windows**
     ```bash
     ./scripts/windows_build_debug.bat
     ```
   *  For **Linux**
      ```bash
      ./scripts/linux_build_debug.sh
      ```

5. Run the game:
   * On **Windows**: Open executable in the `build/bin` or `build/bin` directory.
   * On **Linux**: Run executable 
     ```bash
     ./build/bin/HeronTriangle
     ```

## Notes
* Ensure all dependencies are correctly installed before starting the build process.
* If you encounterr errors, consult the project's issue tracker.

## License
This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Contributing
Contributions are welcome! Please fork the repository and submit a pull request. For major changes, please open an issue first to discuss what you would like to change.


## Authors
- Tymon Woźniak - Creator and Maintainer
