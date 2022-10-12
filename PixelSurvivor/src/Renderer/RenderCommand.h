#pragma once
#include "Shader.h"
#include "VertexArray.h"

namespace Pixel
{

	namespace Detail
	{

		void OnGlError(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);

	}

	enum class CullFace
	{
		None,
		Front = GL_FRONT,
		Back = GL_BACK,
		FrontAndBack = GL_FRONT_AND_BACK,
	};

	class RenderCommand
	{
	private:
		static int s_LastClipPlaneCount;

	public:
		static void Init();
		static void BindDefaultFramebuffer();
		static void SetClearColor(const glm::vec4& color);
		static void Clear();
		static void ClearDepth();
		static void EnableCullFace(bool enabled);
		static void SetCullFace(CullFace face);
		inline static void SetViewport(const Viewport& viewport) { SetViewport(viewport.Left, viewport.Bottom, viewport.Width, viewport.Height); }
		static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
		static void EnableWireframe(bool enable);
		static void DrawIndexed(GLuint drawMode, const VertexArray& vertexArray);
		static void EnableClippingPlanes(int count);
	};

}
