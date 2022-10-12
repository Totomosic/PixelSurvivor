#include "Shader.h"

namespace Pixel
{

	Shader::Shader(const std::string& vertexSource, const std::string& fragmentSource)
		: m_Handle(), m_UniformLocations()
	{
		Init(vertexSource, fragmentSource);
	}

	void Shader::Init(const std::string& vertexSource, const std::string& fragmentSource)
	{
		m_Handle.Id = glCreateProgram();
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		CompileShader(vertexShader, vertexSource);
		CompileShader(fragmentShader, fragmentSource);

		glAttachShader(m_Handle.Id, vertexShader);
		glAttachShader(m_Handle.Id, fragmentShader);
		glLinkProgram(m_Handle.Id);

		GLint success;
		glGetProgramiv(m_Handle.Id, GL_LINK_STATUS, &success);
		if (!success)
		{
			char log[512];
			glGetProgramInfoLog(m_Handle.Id, sizeof(log), nullptr, log);
			PIX_ERROR("Failed linking shader");
			PIX_ASSERT(false, log);
		}

		glDetachShader(m_Handle.Id, vertexShader);
		glDetachShader(m_Handle.Id, fragmentShader);
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}

	void Shader::CompileShader(GLuint shader, const std::string& source)
	{
		const char* src = source.c_str();
		glShaderSource(shader, 1, &src, nullptr);
		glCompileShader(shader);

		GLint success;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			char log[512];
			glGetShaderInfoLog(shader, sizeof(log), nullptr, log);
			PIX_ERROR("Failed compiling shader");
			PIX_ASSERT(false, log);
		}
	}

}
