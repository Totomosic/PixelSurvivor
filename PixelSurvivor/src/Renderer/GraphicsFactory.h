#pragma once
#include "Shader.h"
#include "Texture2D.h"
#include "VertexArray.h"

namespace Pixel
{

	class GraphicsFactory
	{
	public:
		static std::shared_ptr<VertexArray> CreateTexturedQuad(const glm::vec2& size);

		static std::shared_ptr<Shader> CreateScreenQuadShader();
		static std::shared_ptr<Shader> CreateBasicSpriteShader();
	};

}
