#pragma once

#include <vector>

#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

class Mesh {
public:
  Mesh();
private:
  VertexBuffer m_vertex_buffer;
  VertexBufferLayout m_vertex_buffer_layout;
  IndexBuffer m_index_buffer;
};
