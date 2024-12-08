#include "VertexArray.hpp"
#include "Renderer.hpp"
#include "VertexBufferLayout.hpp"

namespace Heron {

VertexArray::VertexArray() {
  GLCall(glGenVertexArrays(1, &m_renderer_id));
}

VertexArray::~VertexArray() {
  remove();
}

void VertexArray::add_buffer(const VertexBuffer& vb, const VertexBufferLayout& layout) const {
  bind();
  vb.bind();
  const auto& elements = layout.get_elements();
  unsigned int offset = 0;
  for (unsigned int i = 0; i < elements.size(); i++) {
    const auto& element = elements[i];
    GLCall(glEnableVertexAttribArray(i));
    GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.get_stride(), (const void*) offset));
    offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
  }
}

void VertexArray::bind() const {
  GLCall(glBindVertexArray(m_renderer_id));
}

void VertexArray::unbind() const {
  GLCall(glBindVertexArray(0));
}

void VertexArray::remove() {
  if (m_renderer_id != 0) {
    GLCall(glDeleteVertexArrays(1, &m_renderer_id));
    m_renderer_id = 0;
  }
}

}