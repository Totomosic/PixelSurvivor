#pragma once
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include <memory>

namespace Pixel
{

	class VertexArray
	{
	private:
		Internal::ScopedHandle<Internal::DeleteVertexArray> m_Handle;
		std::unique_ptr<VertexBuffer> m_Vertices;
		std::unique_ptr<IndexBuffer> m_Indices;

	public:
		VertexArray();

		inline void Bind() const
		{
			glBindVertexArray(m_Handle.Id);
		}

		inline void Unbind() const
		{
			glBindVertexArray(0);
		}

		inline uint32_t GetIndexCount() const
		{
			return m_Indices ? m_Indices->GetIndexCount() : 0;
		}

		void SetVertexBuffer(std::unique_ptr<VertexBuffer>&& vertices);
		void SetIndexBuffer(std::unique_ptr<IndexBuffer>&& indices);

	private:
		void Init();
	};

}
