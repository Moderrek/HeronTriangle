#ifndef RENDERER_HPP_INCLUDED
#define RENDERER_HPP_INCLUDED 1

#include "../Base.hpp" // assert, debugbreak

#include <string>

#include "../Triangle.hpp"
#include "Camera.hpp"

#include <glm/glm.hpp>
#include <GL/glew.h>

#define GLCall(x) ::Heron::Renderer2D::GLClearErrors();\
x;\
ASSERT(::Heron::Renderer2D::GLCheckError(#x, __FILE__, __LINE__))\

namespace Heron {

  class Renderer2D {
  public:
    static void Init();
    static void Shutdown();

    static void BeginScene(const Camera2D& camera);
    static void EndScene();

    static void SetColor(const glm::vec4& color);

    // Drawing
    static void DrawTriangle(const Triangle& triangle, const glm::mat4& projection, const glm::mat4& view, const glm::mat4& model);
    
    static void DrawGrid(const glm::mat4& projection, const glm::mat4& view, const glm::mat4& model);

    static void DrawCircle(const glm::vec2& position, float radius, const glm::mat4& projection, const glm::mat4& view);

    // Errors
    static void GLClearErrors();
    static bool GLCheckError(const char* function, const char* file, int line);
  private:
    static void GenerateGrid();
    static void GenerateCircle(float radius, int segments);
    static void GenerateTriangle();
    static void UpdateTriangleBuffer(const Triangle& triangle);
  };

}

float distance_squared(const glm::vec2& a, const glm::vec2& b);

#endif // RENDERER_HPP_INCLUDED