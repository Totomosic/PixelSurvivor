#pragma once
#include "BufferLayout.h"
#include "Utils.h"

namespace Pixel
{

	class VertexBuffer
	{
	private:
		BufferLayout m_Layout;
		Internal::ScopedHandle<Internal::DeleteBuffer> m_Handle;
		uint32_t m_VertexCount;

	public:
		VertexBuffer(uint32_t vertexCount, const BufferLayout& layout);
		VertexBuffer(const void* data, uint32_t vertexCount, const BufferLayout& layout);

		inline const BufferLayout& GetLayout() const { return m_Layout; }
		inline uint32_t GetVertexCount() const { return m_VertexCount; }

		inline void Bind() const { glBindBuffer(GL_ARRAY_BUFFER, m_Handle.Id); }
		inline void Unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }

	private:
		void Init(const void* data, uint32_t vertexCount);
	};

}
