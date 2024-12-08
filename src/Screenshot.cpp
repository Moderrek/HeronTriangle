#include "Screenshot.hpp"

#include <vector>

// OpenGL
#include <GL/glew.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>

void Screenshot::save(const char *filename, int width, int height) {
  std::vector<unsigned char> pixels(width * height * 3);

  glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels.data());

  std::vector<unsigned char> flippedPixels(width * height * 3);
  for (int y = 0; y < height; ++y) {
    memcpy(&flippedPixels[y * width * 3], &pixels[(height - 1 - y) * width * 3], width * 3);
  }

  stbi_write_png(filename, width, height, 3, flippedPixels.data(), width * 3);
}