#pragma once
#include "Utils.h"
#include <string>

namespace Pixel
{

	struct TextureProps
	{
	public:
		GLenum Format = GL_RGBA;
		GLenum MinFilter = GL_NEAREST;
		GLenum MagFilter = GL_NEAREST;
		GLenum WrapMode = GL_REPEAT;
	};

	struct TextureFullProps
	{
	public:
		int Width = 0;
		int Height = 0;
		GLenum Format = GL_RGBA;
		GLenum InternalFormat = GL_RGBA16F;
		GLenum MinFilter = GL_NEAREST;
		GLenum MagFilter = GL_NEAREST;
		GLenum WrapMode = GL_REPEAT;
	};

	class Texture2D
	{
	private:
		Internal::ScopedHandle<Internal::DeleteTexture> m_Handle;
		TextureFullProps m_Props;

	private:
		Texture2D(const void* pixelData, const TextureFullProps& props);

	public:
		inline GLuint GetId() const { return m_Handle.Id; }

		inline void Bind() const
		{
			glBindTexture(GL_TEXTURE_2D, m_Handle.Id);
		}

		inline void Unbind() const
		{
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		inline void Bind(int sampler) const
		{
			glBindTextureUnit(sampler, m_Handle.Id);
		}

		inline void Unbind(int sampler) const
		{
			glBindTextureUnit(sampler, 0);
		}

		inline void SetWrapMode(GLenum wrapMode)
		{
			m_Props.WrapMode = wrapMode;
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, m_Props.WrapMode);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_Props.WrapMode);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_Props.WrapMode);
		}

	public:
		static Texture2D FromData(const void* pixelData, const TextureFullProps& props);
		static Texture2D FromFile(const std::string& filename, const TextureProps& props);

	private:
		void Init(const void* pixelData, const TextureFullProps& props);
	};

}
