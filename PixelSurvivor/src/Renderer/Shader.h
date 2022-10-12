#pragma once
#include "Utils.h"
#include "Core/Logging.h"
#include <string>
#include <unordered_map>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Pixel
{

	class Shader
	{
	private:
		Internal::ScopedHandle<Internal::DeleteShader> m_Handle;
		mutable std::unordered_map<std::string, int> m_UniformLocations;

	public:
		Shader(const std::string& vertexSource, const std::string& fragmentSource);

		inline void Bind() const { glUseProgram(m_Handle.Id); }
		inline void Unbind() const { glUseProgram(0); }

		inline int GetUniformLocation(const std::string& uniform) const
		{
			auto it = m_UniformLocations.find(uniform);
			if (it != m_UniformLocations.end())
				return it->second;
			int location = glGetUniformLocation(m_Handle.Id, uniform.c_str());
			if (location < 0)
			{
				PIX_WARN("Uniform {} does not exist", uniform);
			}
			m_UniformLocations[uniform] = location;
			return location;
		}

		template<typename T>
		inline void SetUniform(int location, const T& value) const
		{
			static_assert(false, "Invalid shader uniform type");
		}

		template<>
		inline void SetUniform<float>(int location, const float& value) const
		{
			glUniform1f(location, value);
		}

		template<>
		inline void SetUniform<int>(int location, const int& value) const
		{
			glUniform1i(location, value);
		}

		template<>
		inline void SetUniform<glm::vec2>(int location, const glm::vec2& value) const
		{
			glUniform2f(location, value.x, value.y);
		}

		template<>
		inline void SetUniform<glm::vec3>(int location, const glm::vec3& value) const
		{
			glUniform3f(location, value.x, value.y, value.z);
		}

		template<>
		inline void SetUniform<glm::vec4>(int location, const glm::vec4& value) const
		{
			glUniform4f(location, value.x, value.y, value.z, value.w);
		}

		template<>
		inline void SetUniform<glm::mat3>(int location, const glm::mat3& value) const
		{
			glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
		}

		template<>
		inline void SetUniform<glm::mat4>(int location, const glm::mat4& value) const
		{
			glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
		}

	private:
		void Init(const std::string& vertexSource, const std::string& fragmentSource);
		void CompileShader(GLuint shader, const std::string& source);
	};

}
