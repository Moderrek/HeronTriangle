# Heron Triangle

![Linux](https://img.shields.io/badge/Platform-Linux-blue?logo=linux)
![Windows](https://img.shields.io/badge/Platform-Windows-blue?logo=windows)
![OpenGL](https://img.shields.io/badge/Graphics-OpenGL-green?logo=opengl)
![MIT License](https://img.shields.io/badge/License-MIT-yellow?logo=opensourceinitiative)
![GitHub Downloads (all assets, all releases)](https://img.shields.io/github/downloads/Moderrek/HeronTriangle/total)

[Heron's Formula](https://en.wikipedia.org/wiki/Heron%27s_formula) is a method for calculating the **area** of a [triangle](https://en.wikipedia.org/wiki/Triangle) when the lengths of all three sides are known. This formula is attributed to **Heron of Alexandria**.


[**Download Heron Triangle Visualizer (Linux, Windows) HERE**](https://github.com/Moderrek/HeronTriangle/releases)


![image](https://github.com/user-attachments/assets/98017984-4fcb-499d-aae1-badc98fa7711)

---

## Formula Overview

Given the three side lengths of a [triangle](https://en.wikipedia.org/wiki/Triangle):

- ***`a`*** = first side
- ***`b`*** = second side
- ***`c`*** = third side

### Step 1: Calculate the Semi-Perimeter ***`s`***

The [semi-perimeter](https://en.wikipedia.org/wiki/Semiperimeter) is half the [perimeter](https://en.wikipedia.org/wiki/Perimeter) of the [triangle](https://en.wikipedia.org/wiki/Triangle) and is calculated as:

```math
s = \frac{a + b + c}{2}
```

### Step 2: Calculate the [Area](https://en.wikipedia.org/wiki/Area) ***`A`***

The area of the triangle is determined using the following formula:

```math
A = \sqrt{s(s - a)(s - b)(s - c)}
```

---

## Example

### Input:
Let the sides of the triangle be:

- `a = 5`
- `b = 6`
- `c = 7`

### Calculation:
1. Compute the [semi-perimeter](https://en.wikipedia.org/wiki/Semiperimeter):
   ```math
   s = \frac{5 + 6 + 7}{2} = 9
   ```

2. Compute the area:
   ```math
   A = \sqrt{9(9 - 5)(9 - 6)(9 - 7)}
   ```
   ```math
   A = \sqrt{9 \cdot 4 \cdot 3 \cdot 2} = \sqrt{216} \approx 14.7
   ```

### Output:
The area of the triangle is approximately **14.7 square units**.

---

## Applications

- [Geometry](https://en.wikipedia.org/wiki/Geometry) and [trigonometry](https://en.wikipedia.org/wiki/Trigonometry) problems
- Engineering and architecture
- Computational graphics and simulations
- Heron's Formula works for any triangle, as long as the sum of any two sides is greater than the third side ([triangle inequality](https://en.wikipedia.org/wiki/Triangle_inequality)).

To learn more about Heron's formula, [click here](https://en.wikipedia.org/wiki/Heron%27s_formula).

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
* Vertex dragging
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
