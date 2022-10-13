#include "GraphicsFactory.h"

namespace Pixel
{

    std::shared_ptr<VertexArray> GraphicsFactory::CreateTexturedQuad(const glm::vec2& size)
    {
		std::shared_ptr<VertexArray> vertexArray = std::make_shared<VertexArray>();
		glm::vec2 halfSize = size / 2.0f;

		float vertices[] = {
			-halfSize.x, -halfSize.y, 0.0f, 0.0f, 0.0f,
			-halfSize.x, halfSize.y, 0.0f, 0.0f, 1.0f,
			halfSize.x, halfSize.y, 0.0f, 1.0f, 1.0f,
			halfSize.x, -halfSize.y, 0.0f, 1.0f, 0.0f,
		};
		uint32_t indices[] = { 1, 0, 3, 1, 3, 2 };

		Pixel::BufferLayout layout = {
			{ GL_FLOAT, 3 },
			{ GL_FLOAT, 2 },
		};

		vertexArray->SetVertexBuffer(std::make_unique<Pixel::VertexBuffer>(vertices, 4, layout));
		vertexArray->SetIndexBuffer(std::make_unique<Pixel::IndexBuffer>(indices, 6));

		return vertexArray;
    }

	std::shared_ptr<Shader> GraphicsFactory::CreateScreenQuadShader()
	{
		std::string vertexShader =
			"#version 450 core\n"
			"in layout(location = 0) vec3 v_Position;\n"
			"in layout(location = 1) vec2 v_TexCoord;\n"
			"out vec2 f_TexCoord;\n"
			"void main()\n"
			"{\n"
			"   gl_Position = vec4(v_Position, 1.0);\n"
			"   f_TexCoord = v_TexCoord;\n"
			"}\n";

		std::string fragmentShader =
			"#version 450 core\n"
			"in vec2 f_TexCoord;\n"
			"out layout(location = 0) vec4 f_Color;\n"
			"uniform sampler2D u_Texture;\n"
			"void main()\n"
			"{\n"
			"   f_Color = texture(u_Texture, f_TexCoord);\n"
			"}\n";
		return std::make_shared<Shader>(vertexShader, fragmentShader);
	}

	std::shared_ptr<Shader> GraphicsFactory::CreateBasicSpriteShader()
	{
		std::string vertexShader =
			"#version 450 core\n"
			"in layout(location = 0) vec3 v_Position;\n"
			"in layout(location = 1) vec2 v_TexCoord;\n"
			"uniform mat4 u_ModelMatrix;\n"
			"uniform mat4 u_ViewMatrix;\n"
			"uniform mat4 u_ProjectionMatrix;\n"
			"out vec2 f_TexCoord;\n"
			"void main()\n"
			"{\n"
			"   gl_Position = u_ProjectionMatrix * u_ViewMatrix * u_ModelMatrix * vec4(v_Position, 1.0);\n"
			"   f_TexCoord = v_TexCoord;\n"
			"}\n";

		std::string fragmentShader =
			"#version 450 core\n"
			"in vec2 f_TexCoord;\n"
			"out layout(location = 0) vec4 f_Color;\n"
			"uniform sampler2D u_Texture;\n"
			"void main()\n"
			"{\n"
			"   f_Color = texture(u_Texture, f_TexCoord);\n"
			"}\n";
		return std::make_shared<Shader>(vertexShader, fragmentShader);
	}

}
