#pragma once

#include <string>
#include <glad/glad.h>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <fstream>
#include <sstream>
#include <iostream>

#include "Log.h"


namespace Core
{
	class Shader
	{
	public:
		Shader(const std::string& vertexPath, const std::string& fragmentPath);
		~Shader();

		void Use() const;
		void SetMatrix4(const std::string& name, const glm::mat4& matrix) const;
		void SetVec3(const std::string& name, const glm::vec3& value) const;
		void SetFloat(const std::string& name, float value) const;
		void SetBool(const std::string& name, bool value) const;

	private:
		std::string LoadShaderFromFile(const std::string& path);
		void CheckCompileErrors(GLuint shader, const std::string& type);

		GLuint m_ID;
	};
}