#include "Renderer.h"

namespace Pixel
{

	Renderer::Renderer(const Viewport& viewport)
		: m_CurrentScene(), m_Framebuffers(), m_Viewport()
	{
		SetViewport(viewport);
	}

	void Renderer::SetViewport(const Viewport& viewport)
	{
		m_Viewport = viewport;
		m_Framebuffers.clear();
	}

	void Renderer::BeginLayer(const PixelLayer& layer, const CameraData& camera)
	{
		m_CurrentScene.Layer = layer;
		m_CurrentScene.Camera = camera;
		std::shared_ptr<Framebuffer> framebuffer = GetOrCreateFramebuffer(layer.PixelSize);
		m_CurrentScene.RenderTarget = framebuffer;
		InitializeScene(m_CurrentScene);
	}

	void Renderer::EndLayer()
	{
		m_CurrentScene.RenderTarget->Unbind();
		if (!m_ScreenQuad)
			m_ScreenQuad = GraphicsFactory::CreateTexturedQuad({ 2.0f, 2.0f });
		if (!m_ScreenShader)
			m_ScreenShader = GraphicsFactory::CreateScreenQuadShader();
		m_ScreenShader->Bind();
		m_CurrentScene.RenderTarget->GetColorAttachment(0)->Bind(0);
		m_ScreenShader->SetUniform(m_ScreenShader->GetUniformLocation("u_Texture"), 0);

		RenderCommand::SetViewport(m_Viewport);
		RenderCommand::SetClearColor({ 1.0f, 0.0f, 1.0f, 1.0f });
		RenderCommand::Clear();
		RenderCommand::DrawIndexed(GL_TRIANGLES, *m_ScreenQuad);
	}

	void Renderer::DrawSprite(const glm::vec3& center, const glm::vec2& size, const Texture2D* texture, SpriteOptions options)
	{
		glm::vec3 adjustedPos = center;
		glm::vec2 adjustedSize = size;
		if (options.SnapToPixelGrid)
		{
			adjustedPos.x = int(adjustedPos.x / m_CurrentScene.Layer.PixelSize) * m_CurrentScene.Layer.PixelSize;
			adjustedPos.y = int(adjustedPos.y / m_CurrentScene.Layer.PixelSize) * m_CurrentScene.Layer.PixelSize;
			adjustedSize.x = int(adjustedSize.x / m_CurrentScene.Layer.PixelSize) * m_CurrentScene.Layer.PixelSize;
			adjustedSize.y = int(adjustedSize.y / m_CurrentScene.Layer.PixelSize) * m_CurrentScene.Layer.PixelSize;
		}
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), adjustedPos) * glm::scale(glm::mat4(1.0f), { adjustedSize.x, adjustedSize.y, 1.0f });
		m_SpriteShader->SetUniform(m_SpriteShader->GetUniformLocation("u_ModelMatrix"), transform);

		texture->Bind(0);
		m_SpriteShader->SetUniform(m_SpriteShader->GetUniformLocation("u_Texture"), 0);

		RenderCommand::DrawIndexed(GL_TRIANGLES, *m_Quad);
	}

	void Renderer::InitializeScene(const SceneData& scene)
	{
		scene.RenderTarget->Bind();
		RenderCommand::SetViewport(scene.RenderTarget->GetViewport());
		RenderCommand::SetClearColor(scene.Camera.ClearColor);
		RenderCommand::Clear();
		if (!m_Quad)
			m_Quad = GraphicsFactory::CreateTexturedQuad({ 1.0f, 1.0f });
		if (!m_SpriteShader)
			m_SpriteShader = GraphicsFactory::CreateBasicSpriteShader();
		m_SpriteShader->Bind();
		m_SpriteShader->SetUniform(m_SpriteShader->GetUniformLocation("u_ProjectionMatrix"), scene.Camera.ProjectionMatrix);
		m_SpriteShader->SetUniform(m_SpriteShader->GetUniformLocation("u_ViewMatrix"), scene.Camera.ViewMatrix);
	}

	std::shared_ptr<Framebuffer> Renderer::GetOrCreateFramebuffer(int pixelSize)
	{
		auto it = m_Framebuffers.find(pixelSize);
		if (it != m_Framebuffers.end())
			return it->second;
		FramebufferProps props;
		props.Width = m_Viewport.Width / pixelSize;
		props.Height = m_Viewport.Height / pixelSize;
		props.Attachments = {
			{ FramebufferTextureFormat::RGBA16F },
		};
		std::shared_ptr<Framebuffer> framebuffer = std::make_shared<Framebuffer>(Framebuffer::Create(props));
		m_Framebuffers[pixelSize] = framebuffer;
		return framebuffer;
	}

}
