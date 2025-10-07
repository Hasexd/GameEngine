#include "Shader.h"

namespace Core
{
	Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath):
		m_VertexShaderName(FileUtils::GetFileName(vertexPath)), m_FragmentShaderName(FileUtils::GetFileName(fragmentPath))
	{
		m_ID = Create();
	}

	bool Shader::Reload()
	{
		GLuint reloadedProgram = Create();

		if (reloadedProgram != -1)
		{
			glDeleteProgram(m_ID);
			m_ID = reloadedProgram;

			return true;
		}

		return false;
	}

	GLuint Shader::Create()
	{
		std::string vertexCode = LoadShaderFromFile(FileUtils::GetShaderPath(m_VertexShaderName));
		std::string fragmentCode = LoadShaderFromFile(FileUtils::GetShaderPath(m_FragmentShaderName));

		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();

		GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, nullptr);
		glCompileShader(vertex);

		if (!CheckCompileErrors(vertex, "VERTEX"))
		{
			glDeleteShader(vertex);
			return -1;
		}

		GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, nullptr);
		glCompileShader(fragment);
		
		if (!CheckCompileErrors(fragment, "FRAGMENT"))
		{
			glDeleteShader(vertex);
			glDeleteShader(fragment);
			return -1;
		}

		GLuint programID = glCreateProgram();
		glAttachShader(programID, vertex);
		glAttachShader(programID, fragment);
		glLinkProgram(programID);

		if (!CheckCompileErrors(programID, "PROGRAM"))
		{
			glDeleteShader(vertex);
			glDeleteShader(fragment);
			glDeleteProgram(programID);

			return -1;
		}

		glDeleteShader(vertex);
		glDeleteShader(fragment);

		return programID;
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

	void Shader::SetBool(const std::string& name, bool value) const
	{
		glUniform1i(glGetUniformLocation(m_ID, name.c_str()), static_cast<int>(value));
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

	bool Shader::CheckCompileErrors(GLuint shader, const std::string& type)
	{
		int success;

		char infoLog[1024];

		if (type != "PROGRAM")
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

			if (!success)
			{
				glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
				LOG_ERROR("shader compilation error of type: {}\n{}", type, infoLog);

				return false;
			}
		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &success);

			if (!success)
			{
				glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
				LOG_ERROR("Program linking error of type: {}\n{}", type, infoLog);

				return false;
			}
		}

		return true;
	}

	Shader::~Shader()
	{
		glDeleteProgram(m_ID);
	}
}