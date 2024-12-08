#include "VertexBuffer.hpp"
#include "Renderer.hpp"

VertexBuffer::VertexBuffer(const void* data, unsigned int size) {
  GLCall(glGenBuffers(1, &m_renderer_id));
  GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id));
  GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer() {
  GLCall(glDeleteBuffers(1, &m_renderer_id));
}

void VertexBuffer::bind() const {
  GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id));
}

void VertexBuffer::unbind() const {
  GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}