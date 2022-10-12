#pragma once
#include <glad/glad.h>
#include <algorithm>

namespace Pixel
{

	namespace Internal
	{

		struct DeleteBuffer
		{
		public:
			inline void operator()(GLuint id) const
			{
				glDeleteBuffers(1, &id);
			}
		};

		struct DeleteVertexArray
		{
		public:
			inline void operator()(GLuint id) const
			{
				glDeleteVertexArrays(1, &id);
			}
		};

		struct DeleteShader
		{
		public:
			inline void operator()(GLuint id) const
			{
				glDeleteProgram(id);
			}
		};

		struct DeleteTexture
		{
		public:
			inline void operator()(GLuint id) const
			{
				glDeleteTextures(1, &id);
			}
		};

		struct DeleteFramebuffer
		{
		public:
			inline void operator()(GLuint id) const
			{
				glDeleteFramebuffers(1, &id);
			}
		};

		template<typename T>
		class ScopedHandle
		{
		public:
			GLuint Id = 0;

		public:
			ScopedHandle() = default;
			ScopedHandle(const ScopedHandle<T>& other) = delete;
			ScopedHandle<T>& operator=(const ScopedHandle<T>& other) = delete;
			inline ScopedHandle(ScopedHandle<T>&& other)
				: Id(other.Id)
			{
				other.Id = 0;
			}

			inline ScopedHandle<T>& operator=(ScopedHandle<T>&& other)
			{
				std::swap(Id, other.Id);
				return *this;
			}

			~ScopedHandle()
			{
				if (Id != 0)
				{
					T{}(Id);
				}
			}
		};

	}

	struct Viewport
	{
	public:
		int Left = 0;
		int Bottom = 0;
		int Width = 0;
		int Height = 0;
	};

}
