#pragma once
#include "RenderCommand.h"
#include "GraphicsFactory.h"
#include "Framebuffer.h"

namespace Pixel
{

	struct PixelLayer
	{
	public:
		int PixelSize = 0;
	};

	struct CameraData
	{
	public:
		glm::mat4 ViewMatrix = glm::mat4(1.0f);
		glm::mat4 ProjectionMatrix = glm::mat4(1.0f);
		glm::vec4 ClearColor = glm::vec4{ 0.0f, 0.0f, 0.0f, 0.0f };
	};

	struct SceneData
	{
	public:
		PixelLayer Layer;
		CameraData Camera;
		std::shared_ptr<Framebuffer> RenderTarget;
	};

	struct SpriteOptions
	{
	public:
		bool SnapToPixelGrid = true;
	};

	class Renderer
	{
	private:
		SceneData m_CurrentScene;
		std::unordered_map<int, std::shared_ptr<Framebuffer>> m_Framebuffers;
		Viewport m_Viewport;

		std::shared_ptr<VertexArray> m_Quad;
		std::shared_ptr<VertexArray> m_ScreenQuad;
		std::shared_ptr<Shader> m_ScreenShader;
		std::shared_ptr<Shader> m_SpriteShader;

	public:
		Renderer(const Viewport& viewport);

		void SetViewport(const Viewport& viewport);

		void BeginLayer(const PixelLayer& layer, const CameraData& camera);
		void EndLayer();

		void DrawSprite(const glm::vec3& center, const glm::vec2& size, const Texture2D* texture, SpriteOptions options = {});

	private:
		void InitializeScene(const SceneData& scene);
		std::shared_ptr<Framebuffer> GetOrCreateFramebuffer(int pixelSize);
	};

}
