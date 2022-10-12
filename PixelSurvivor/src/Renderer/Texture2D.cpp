#include "Texture2D.h"
#include "Core/Logging.h"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image.h>

namespace Pixel
{
	Texture2D::Texture2D(const void* pixelData, const TextureFullProps& props)
		: m_Handle(), m_Props(props)
	{
		Init(pixelData, m_Props);
	}

	Texture2D Texture2D::FromData(const void* pixelData, const TextureFullProps& props)
	{
		return Texture2D(pixelData, props);
	}

	Texture2D Texture2D::FromFile(const std::string& filename, const TextureProps& props)
	{
		TextureFullProps fullProps;
		fullProps.Format = props.Format;
		fullProps.MinFilter = props.MinFilter;
		fullProps.MagFilter = props.MagFilter;
		fullProps.WrapMode = props.WrapMode;
		int channels;
		stbi_set_flip_vertically_on_load(true);
		unsigned char* pixels = stbi_load(filename.c_str(), &fullProps.Width, &fullProps.Height, &channels, 4);
		PIX_ASSERT(pixels != nullptr, "Failed to load texture {}", filename);
		Texture2D texture(pixels, fullProps);
		stbi_image_free(pixels);
		return texture;
	}

	void Texture2D::Init(const void* pixelData, const TextureFullProps& props)
	{
		glCreateTextures(GL_TEXTURE_2D, 1, &m_Handle.Id);
		glBindTexture(GL_TEXTURE_2D, m_Handle.Id);
		glTexImage2D(GL_TEXTURE_2D, 0, props.InternalFormat, props.Width, props.Height, 0, props.Format, GL_UNSIGNED_BYTE, pixelData);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, props.MinFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, props.MagFilter);
		SetWrapMode(props.WrapMode);
	}

}
