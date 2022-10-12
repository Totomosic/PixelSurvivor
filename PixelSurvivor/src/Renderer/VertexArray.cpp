#include "VertexArray.h"

namespace Pixel
{
	VertexArray::VertexArray()
		: m_Handle()
	{
		Init();
	}

	void VertexArray::SetVertexBuffer(std::unique_ptr<VertexBuffer>&& vertices)
	{
		Bind();
		vertices->Bind();

		const BufferLayout& layout = vertices->GetLayout();
		uint32_t attributeIndex = 0;

		for (const AttributeDescriptor& attribute : layout.Attributes)
		{
			glEnableVertexAttribArray(attributeIndex);
			glVertexAttribPointer(attributeIndex, attribute.Count, attribute.DataType, attribute.Normalized, layout.Stride, (const void*)attribute.Offset);
			attributeIndex++;
		}

		m_Vertices = std::move(vertices);
	}

	void VertexArray::SetIndexBuffer(std::unique_ptr<IndexBuffer>&& indices)
	{
		Bind();
		indices->Bind();
		m_Indices = std::move(indices);
	}

	void VertexArray::Init()
	{
		glGenVertexArrays(1, &m_Handle.Id);
	}

}
