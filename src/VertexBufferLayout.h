#pragma once

#include <GL/glew.h>

#include <vector>

struct VertexBufferElement {
  unsigned int type;
  unsigned int count;
  unsigned char normalized;

  VertexBufferElement(unsigned int _type, unsigned int _count, unsigned int _normalized) {
    type = _type;
    count = _count;
    normalized = _normalized;
  }

  static unsigned int GetSizeOfType(unsigned int type) {
    switch (type) {
    case GL_FLOAT: return 4;
    case GL_UNSIGNED_INT: return 4;
    case GL_UNSIGNED_BYTE: return 1;
    }
    return 0;
  }
};

class VertexBufferLayout {
public:
  VertexBufferLayout() : m_stride{0} {}

  template <typename T>
  void push(unsigned int count) {
    static_assert(false);
  }

  template <>
  void push<float>(unsigned int count) {
    m_elements.emplace_back(GL_FLOAT, count, GL_FALSE);
    m_stride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
  }

  template <>
  void push<unsigned int>(unsigned int count) {
    m_elements.emplace_back(GL_UNSIGNED_INT, count, GL_FALSE);
    m_stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
  }

  template <>
  void push<unsigned char>(unsigned int count) {
    m_elements.emplace_back(GL_UNSIGNED_BYTE, count, GL_TRUE);
    m_stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);
  }

  inline const std::vector<VertexBufferElement>& get_elements() const {
    return m_elements;
  }

  inline unsigned int get_stride() const {
    return m_stride;
  }

private:
  std::vector<VertexBufferElement> m_elements;
  unsigned int m_stride;
};
