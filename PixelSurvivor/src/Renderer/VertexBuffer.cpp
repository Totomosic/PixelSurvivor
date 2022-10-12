#include "VertexBuffer.h"

namespace Pixel
{

	VertexBuffer::VertexBuffer(uint32_t vertexCount, const BufferLayout& layout) : VertexBuffer(nullptr, vertexCount, layout)
	{
	}

	VertexBuffer::VertexBuffer(const void* data, uint32_t vertexCount, const BufferLayout& layout)
		: m_Layout(layout), m_Handle(), m_VertexCount(vertexCount)
	{
		Init(data, vertexCount);
	}

	void VertexBuffer::Init(const void* data, uint32_t vertexCount)
	{
		glGenBuffers(1, &m_Handle.Id);
		Bind();
		glBufferData(GL_ARRAY_BUFFER, vertexCount * m_Layout.Stride, data, GL_STATIC_DRAW);
		Unbind();
	}

}
