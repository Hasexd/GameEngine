#include "Shader.h"

namespace Core
{
	Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
	{
		std::string vertexCode = LoadShaderFromFile(vertexPath);
		std::string fragmentCode = LoadShaderFromFile(fragmentPath);

		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();

		GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, nullptr);
		glCompileShader(vertex);
		CheckCompileErrors(vertex, "VERTEX");

		GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, nullptr);
		glCompileShader(fragment);
		CheckCompileErrors(fragment, "FRAGMENT");

		m_ID = glCreateProgram();
		glAttachShader(m_ID, vertex);
		glAttachShader(m_ID, fragment);
		glLinkProgram(m_ID);
		CheckCompileErrors(m_ID, "PROGRAM");

		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}

	void Shader::Use() const
	{
		glUseProgram(m_ID);
	}

	void Shader::SetMatrix4(const std::string& name, const glm::mat4& matrix) const
	{
		glUniformMatrix4fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void Shader::SetVec3(const std::string& name, const glm::vec3& value) const
	{
		glUniform3fv(glGetUniformLocation(m_ID, name.c_str()), 1, glm::value_ptr(value));
	}

	void Shader::SetFloat(const std::string& name, float value) const
	{
		glUniform1f(glGetUniformLocation(m_ID, name.c_str()), value);
	}

	std::string Shader::LoadShaderFromFile(const std::string& path)
	{
		std::string shaderCode;
		std::ifstream shaderFile;

		shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try
		{
			shaderFile.open(path);

			std::stringstream stream;
			stream << shaderFile.rdbuf();

			shaderFile.close();

			shaderCode = stream.str();
		}
		catch (std::ifstream::failure& e)
		{
			LOG_ERROR("Failed to read shader file: {}", path);
		}

		return shaderCode;
	}

	void Shader::CheckCompileErrors(GLuint shader, const std::string& type)
	{
		int success;
		char infoLog[1024];

		if (type != "PROGRAM")
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

			if (!success)
			{
				glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
				LOG_ERROR("shader compilation error of type: {}\n{}", type, infoLog);
			}
		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &success);

			if (!success)
			{
				glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
				LOG_ERROR("Program linking error of type: {}\n{}", type, infoLog);
			}
		}
	}

	Shader::~Shader()
	{
		glDeleteProgram(m_ID);
	}
}