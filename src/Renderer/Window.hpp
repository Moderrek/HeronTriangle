#ifndef WINDOW_HPP_INCLUDED
#define WINDOW_HPP_INCLUDED 1

#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace Heron {

  class Window {
  public:
    Window(const std::string& title, unsigned int width, unsigned int height);
    ~Window();

    void on_update();

    unsigned int get_width() const;
    unsigned int get_height() const;

    void set_vsync(const bool enabled);
    bool is_vsync() const;

  private:
    GLFWwindow* m_window;

    std::string m_title;
    unsigned int m_width, m_height;
    bool m_vsync;
  };

}

#endif // WINDOW_HPP_INCLUDED