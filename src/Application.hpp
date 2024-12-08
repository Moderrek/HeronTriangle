#ifndef APPLICATION_HPP_INCLUDED
#define APPLICATION_HPP_INCLUDED 1

#include "Renderer/Window.hpp"
#include "Renderer/Renderer.hpp"

namespace Heron {

  class Application {
  public:
    Application();
    ~Application();

    void close();

  private:
    bool m_running = true;
    Window m_window;
  };

}

#endif // APPLICATION_HPP_INCLUDED