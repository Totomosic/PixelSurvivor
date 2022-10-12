#include "RenderCommand.h"

namespace Pixel
{

	namespace Detail
	{
		void OnGlError(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
		{
			switch (severity)
			{
			case GL_DEBUG_SEVERITY_HIGH: PIX_ASSERT(false, message); return;
			case GL_DEBUG_SEVERITY_MEDIUM: PIX_ERROR(message); return;
			case GL_DEBUG_SEVERITY_LOW: PIX_WARN(message); return;
			case GL_DEBUG_SEVERITY_NOTIFICATION: PIX_INFO(message); return;
			}
		}
	}

	int RenderCommand::s_LastClipPlaneCount = 0;

	void RenderCommand::Init()
	{
#ifndef PIX_DIST
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(Detail::OnGlError, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
#endif

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void RenderCommand::BindDefaultFramebuffer()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void RenderCommand::SetClearColor(const glm::vec4& color)
	{
		glClearColor(float(color.r), float(color.g), float(color.b), float(color.a));
	}

	void RenderCommand::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void RenderCommand::ClearDepth()
	{
		glClear(GL_DEPTH_BUFFER_BIT);
	}

	void RenderCommand::EnableCullFace(bool enabled)
	{
		if (enabled)
			glEnable(GL_CULL_FACE);
		else
			glDisable(GL_CULL_FACE);
	}

	void RenderCommand::SetCullFace(CullFace face)
	{
		if (face != CullFace::None)
		{
			glCullFace((GLenum)face);
		}
	}

	void RenderCommand::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}

	void RenderCommand::EnableWireframe(bool enable)
	{
		if (enable)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	void RenderCommand::DrawIndexed(GLuint drawMode, const VertexArray& vertexArray)
	{
		uint32_t count = vertexArray.GetIndexCount();
		vertexArray.Bind();
		glDrawElements(drawMode, count, GL_UNSIGNED_INT, nullptr);
	}

	void RenderCommand::EnableClippingPlanes(int count)
	{
		for (int i = s_LastClipPlaneCount; i < count; i++)
		{
			glEnable(GL_CLIP_DISTANCE0 + i);
		}
		for (int i = count; i < s_LastClipPlaneCount; i++)
		{
			glDisable(GL_CLIP_DISTANCE0 + i);
		}
		s_LastClipPlaneCount = count;
	}

}
