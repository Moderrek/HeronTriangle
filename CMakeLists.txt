cmake_minimum_required(VERSION 3.16)
project(HeronTriangle LANGUAGES CXX)

# C++ Standard settings
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (MINGW)
    message(STATUS = "Detected MinGW")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++")
endif ()


# Set the directory for intermediate and build files
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/bin)     # Executables
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/lib)    # Shared libraries
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/lib)    # Static libraries
set(CMAKE_OBJECT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/obj)            # Object files

# Organize intermediate files in build/temp/
set(CMAKE_TEMP_DIR ${CMAKE_SOURCE_DIR}/build/temp)
set(CMAKE_BINARY_DIR ${CMAKE_TEMP_DIR})  # All temporary files like .dir, .pdb, ZERO_CHECK, etc.

# Debug and Release-specific outputs
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

# Hide CMake artifacts in Visual Studio
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMake Internals")

# GLEW config
set(glew-cmake_BUILD_SHARED OFF CACHE BOOL "Disable building shared GLEW libraries")
set(glew-cmake_BUILD_STATIC ON CACHE BOOL "Enable building static GLEW libraries")
set(ONLY_LIBS ON CACHE BOOL "Disable building GLEW executables")

# Submodules
set(GLFW_DIR "${CMAKE_SOURCE_DIR}/lib/glfw")
set(GLEW_DIR "${CMAKE_SOURCE_DIR}/lib/glew")
set(GLM_DIR "${CMAKE_SOURCE_DIR}/lib/glm")
set(IMGUI_DIR "${CMAKE_SOURCE_DIR}/lib/imgui")

add_subdirectory(${GLM_DIR})
add_subdirectory(${GLFW_DIR})
add_subdirectory(${GLEW_DIR})

# Add source files
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h" "src/*.hpp")
add_executable(HeronTriangle ${SOURCES})

# Add ImGUI source
set(IMGUI_SOURCES
        ${IMGUI_DIR}/imgui.cpp
        ${IMGUI_DIR}/imgui_draw.cpp
        ${IMGUI_DIR}/imgui_tables.cpp
        ${IMGUI_DIR}/imgui_widgets.cpp
        ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
        ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)
target_sources(HeronTriangle PRIVATE ${IMGUI_SOURCES})

target_compile_definitions(HeronTriangle PRIVATE IMGUI_IMPL_OPENGL_LOADER_GLEW)

# Include directories
target_include_directories(
        HeronTriangle PRIVATE
        include/
        lib/glfw/include
        lib/glm
        lib/imgui
        lib/imgui/backends
)

# Link libraries
find_package(OpenGL REQUIRED)
target_link_libraries(HeronTriangle PRIVATE glfw libglew_static OpenGL::GL)

# Copy resources after build
add_custom_command(
        TARGET HeronTriangle POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:HeronTriangle>/assets
        COMMENT "Copy assets to build directory."
)