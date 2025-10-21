#pragma once

#include <glad/glad.h>
#include <vector>
#include <filesystem>
#include <glm.hpp>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>

#include "Component.h"
#include "Log.h"

namespace Core
{
	struct Vertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;
	};

	struct MeshData
	{
		std::vector<Vertex> Vertices;
		std::vector<uint32_t> Indices;
	};

	struct Mesh : Component
	{

		Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);
		Mesh(const MeshData& meshData);
		~Mesh();

		static MeshData LoadFromObj(const std::filesystem::path& path);
		static Mesh CreateFromObj(const std::filesystem::path & path);

		unsigned int VAO, VBO, EBO;
		size_t IndexCount;
	};
}

