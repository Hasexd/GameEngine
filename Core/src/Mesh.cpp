#include "Mesh.h"
#include <unordered_map>

namespace Core
{
	Mesh::Mesh(const std::vector<float>& vertices, const std::vector<uint32_t>& indices) :
		IndexCount(indices.size())
	{
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndexCount * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glBindVertexArray(0);
	}

	Mesh::Mesh(const MeshData& meshData)
	{
		std::vector<float> vertices;
		std::vector<uint32_t> indices;

		meshData.ToInterleaved(vertices, indices);

		IndexCount = indices.size();

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndexCount * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glBindVertexArray(0);
	}

	Mesh::~Mesh()
	{
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
	}

	void MeshData::ToInterleaved(std::vector<float>& outVertices, std::vector<uint32_t>& outIndices) const
	{
		struct VertexKey
		{
			uint32_t posIdx, normIdx, uvIdx;

			bool operator==(const VertexKey& other) const {
				return posIdx == other.posIdx && normIdx == other.normIdx && uvIdx == other.uvIdx;
			}
		};

		struct VertexKeyHash
		{
			size_t operator()(const VertexKey& k) const {
				return ((size_t)k.posIdx << 32) | ((size_t)k.normIdx << 16) | (size_t)k.uvIdx;
			}
		};

		std::unordered_map<VertexKey, uint32_t, VertexKeyHash> uniqueVertices;
		outVertices.clear();
		outIndices.clear();

		for (size_t i = 0; i < VertexIndices.size(); i++)
		{
			VertexKey key = {
				VertexIndices[i],
				NormalIndices.empty() ? 0 : NormalIndices[i],
				UvIndices.empty() ? 0 : UvIndices[i]
			};

			auto it = uniqueVertices.find(key);
			if (it != uniqueVertices.end())
			{
				outIndices.push_back(it->second);
			}
			else
			{
				uint32_t newIndex = (uint32_t)(outVertices.size() / 6);

				const glm::vec3& pos = Vertices[key.posIdx];
				outVertices.push_back(pos.x);
				outVertices.push_back(pos.y);
				outVertices.push_back(pos.z);

				if (!Normals.empty() && key.normIdx < Normals.size())
				{
					const glm::vec3& norm = Normals[key.normIdx];
					outVertices.push_back(norm.x);
					outVertices.push_back(norm.y);
					outVertices.push_back(norm.z);
				}
				else
				{
					outVertices.push_back(0.0f);
					outVertices.push_back(0.0f);
					outVertices.push_back(0.0f);
				}

				uniqueVertices[key] = newIndex;
				outIndices.push_back(newIndex);
			}
		}
	}

	MeshData Mesh::LoadFromObj(const std::filesystem::path& path)
	{
		if (path.extension().string() != ".obj")
		{
			LOG_ERROR("Unsupported file format: {}", path.extension().string());
			return {};
		}

		std::ifstream file(path);

		if (!file.is_open())
		{
			LOG_ERROR("Failed to open file: {}", path.string());
			return {};
		}

		MeshData meshData;

		std::vector<uint32_t> vertexIndices, uvIndices, normalIndices;
		std::vector<glm::vec3> tempVertices, tempNormals;
		std::vector<glm::vec2> tempUvs;

		std::string line, lineHeader;

		while (std::getline(file, line))
		{
			std::istringstream lineStream(line);
			lineStream >> lineHeader;

			if (lineHeader == "v")
			{
				glm::vec3 vertex = {};
				lineStream >> vertex.x >> vertex.y >> vertex.z;

				tempVertices.push_back(vertex);
			}
			else if (lineHeader == "vt")
			{
				glm::vec2 uv = {};
				lineStream >> uv.x >> uv.y;

				tempUvs.push_back(uv);
			}
			else if (lineHeader == "vn")
			{
				glm::vec3 normal = {};
				lineStream >> normal.x >> normal.y >> normal.z;

				tempNormals.push_back(normal);
			}
			else if (lineHeader == "f")
			{
				uint32_t vertexIndex[3] = {}, uvIndex[3] = {}, normalIndex[3] = {};

				std::string vertex1, vertex2, vertex3;
				lineStream >> vertex1 >> vertex2 >> vertex3;

				auto parseVertex = [](std::string& vertexStr, uint32_t& v, uint32_t& uv, uint32_t& n)
					{
						std::replace(vertexStr.begin(), vertexStr.end(), '/', ' ');
						std::istringstream vStream(vertexStr);
						vStream >> v;

						if (vStream >> uv)
						{
							vStream >> n;
						}
						if (v > 0) v--;
						if (uv > 0) uv--;
						if (n > 0) n--;
					};

				parseVertex(vertex1, vertexIndex[0], uvIndex[0], normalIndex[0]);
				parseVertex(vertex2, vertexIndex[1], uvIndex[1], normalIndex[1]);
				parseVertex(vertex3, vertexIndex[2], uvIndex[2], normalIndex[2]);

				vertexIndices.push_back(vertexIndex[0]);
				vertexIndices.push_back(vertexIndex[1]);
				vertexIndices.push_back(vertexIndex[2]);
				uvIndices.push_back(uvIndex[0]);
				uvIndices.push_back(uvIndex[1]);
				uvIndices.push_back(uvIndex[2]);
				normalIndices.push_back(normalIndex[0]);
				normalIndices.push_back(normalIndex[1]);
				normalIndices.push_back(normalIndex[2]);
			}
		}
		file.close();

		meshData.Vertices = tempVertices;
		meshData.Normals = tempNormals;
		meshData.Uvs = tempUvs;

		meshData.VertexIndices = vertexIndices;
		meshData.NormalIndices = normalIndices;
		meshData.UvIndices = uvIndices;

		return meshData;
	}
}