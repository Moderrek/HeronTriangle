#include "Window.hpp"

#include <iostream>

namespace Heron {

  static bool s_initialized_glfw = false;

  static void GLFWErrorCallback(int error, const char* message) {
    std::cerr << "ERROR: GLFW: (" << error << ") " << message << '\n';
  }

  Window::Window(const std::string& title, unsigned int width, unsigned int height) {
    std::cout << "INFO: creating window: \"" << title << "\" with size " << width << 'x' << height << '\n';

    m_title = title;
    m_width = width;
    m_height = height;

    if (!s_initialized_glfw) {
      if (!glfwInit()) {
        std::cerr << "FATAL: Failed to initialize GLFW\n";
        return;
      }
      // TODO error handling
      glfwSetErrorCallback(GLFWErrorCallback);
    }
    std::cout << "INFO: Initialized GLFW\n";


    m_window = glfwCreateWindow((int) m_width, (int) m_height, m_title.c_str(), nullptr, nullptr);
    if (!m_window) {
      std::cerr << "FATAL: Failed to open GLFW window.\n";
      return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    std::cout << "INFO: Created GLFW Window\n";

    glfwMakeContextCurrent(m_window);

    GLenum err = glewInit();
    if (err != GLEW_OK) {
      std::cerr << "FATAL: Failed to initialize GLEW: " << glewGetErrorString(err) << "\n";
      return;
    }
    std::cout << "INFO: Initialized GLEW and OpenGL\n";

    std::cout << "OpenGL:\n"
     << "  Drivers: OpenGL " << glGetString(GL_VERSION) << '\n'
     << "  Vendor: " << glGetString(GL_VENDOR) << '\n'
     << "  Renderer: " << glGetString(GL_RENDERER) << '\n'
     << "  GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << '\n';


    set_vsync(true);
    std::cout << "INFO: Constructed Window\n";
  }

  Window::~Window() {
    if (m_window) glfwDestroyWindow(m_window);
    if (s_initialized_glfw) glfwTerminate();
  }

  void Window::on_update() {
    glfwPollEvents();
    glfwSwapBuffers(m_window);
  }

  unsigned int Window::get_width() const {
    return m_width;
  }

  unsigned int Window::get_height() const {
    return m_height;
  }

  void Window::set_vsync(const bool enabled) {
    if (enabled)
      glfwSwapInterval(1);
    else
      glfwSwapInterval(0);
    m_vsync = enabled;
  }
  
  bool Window::is_vsync() const {
    return m_vsync;
  }

}