#pragma once
#include <glad/glad.h>
#include <stdint.h>
#include <vector>

#include "Core/Logging.h"

namespace Pixel
{

	inline uint32_t CalculateTypeByteSize(GLuint type)
	{
		switch (type)
		{
		case GL_FLOAT:
			return 4;
		case GL_INT:
			return 4;
		}
		PIX_ASSERT(false, "Invalid GLType");
		return 0;
	}

	struct AttributeDescriptor
	{
	public:
		GLuint DataType = GL_FLOAT;
		int Count = 3;
		bool Normalized = false;
		uint32_t Offset = 0;
	};

	struct BufferLayout
	{
	public:
		std::vector<AttributeDescriptor> Attributes;
		uint32_t Stride = 0;

	public:
		inline BufferLayout(std::initializer_list<AttributeDescriptor> attributes)
			: Attributes()
		{
			for (AttributeDescriptor attribute : attributes)
			{
				uint32_t byteSize = CalculateTypeByteSize(attribute.DataType) * attribute.Count;
				attribute.Offset = Stride;
				Stride += byteSize;
				Attributes.push_back(attribute);
			}
		}
	};

}
