#pragma once
#include <glad/glad.h>
#include <stdint.h>

namespace Pixel
{

	class IndexBuffer
	{
	private:
		uint32_t m_BufferId;
		uint32_t m_IndexCount;

	public:
		IndexBuffer(uint32_t indexCount);
		IndexBuffer(const uint32_t* indices, uint32_t indexCount);

		inline uint32_t GetIndexCount() const { return m_IndexCount; }

		inline void Bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferId); }
		inline void Unbind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

	private:
		void Init(const uint32_t* indices, uint32_t indexCount);
	};

}
