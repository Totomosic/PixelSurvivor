#include "IndexBuffer.h"

namespace Pixel
{

	IndexBuffer::IndexBuffer(uint32_t indexCount) : IndexBuffer(nullptr, indexCount)
	{
	}

	IndexBuffer::IndexBuffer(const uint32_t* indices, uint32_t indexCount)
		: m_BufferId(0), m_IndexCount(indexCount)
	{
		Init(indices, indexCount);
	}

	void IndexBuffer::Init(const uint32_t* indices, uint32_t indexCount)
	{
		glGenBuffers(1, &m_BufferId);
		Bind();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(uint32_t), indices, GL_STATIC_DRAW);
		Unbind();
	}

}
