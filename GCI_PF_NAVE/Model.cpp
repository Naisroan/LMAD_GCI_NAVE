#include "model.h"

Model::Model() 
{ 
}

Model::~Model() { }

GLvoid Model::Draw(Shader* shader, mat4 view, mat4 projection)
{
	for (Group& group : groups)
	{
		group.Draw(shader, materials, GetTransformMatrix(), view, projection);
	}
}

Model* Model::CargarOBJ(Shader* shader, string rutaObj, string nombreObj) 
{
	string nombreMtl = "";

	vector<Group> _groups = CargarModelo(shader, rutaObj, nombreObj, nombreMtl);
	vector<Material> _materials = CargarMTL(rutaObj, nombreMtl);

	GLuint index = 0;

	for (Group& group : _groups) 
	{
		for (Mesh& mesh : group.meshes) 
		{
			for (Material& material : _materials) 
			{
				if (mesh.materialName == material.name)
				{
					mesh.materialIndex = index;
					break;
				}

				index++;
			}

			index = 0;
		}
	}

	Model* _model = new Model();
	_model->groups = _groups;
	_model->materials = _materials;

	return _model;
}

vector<Group> Model::CargarModelo(Shader* shader, string rutaObj, string nombreObj, string& nombreMtl)
{
	vector<Group> groupList;

	vector<Mesh::Vertex> vertexBuffer;
	vector<GLuint> indexBuffer;

	vector<vec3> positions;
	vector<vec3> normals;
	vector<vec2> texCoords;

	ifstream file;
	file.open(rutaObj + "/" + nombreObj);

	if (file.is_open())
	{
		while (file.good())
		{
			string token;
			file >> token;

			if (token == "mtllib")
			{
				file >> nombreMtl;
			}
			else if (token == "g")
			{
				if (!groupList.empty())
				{
					groupList.back().meshes.back().size = indexBuffer.size() - groupList.back().meshes.back().offset;

					CalcularTangentes(vertexBuffer, indexBuffer);
					GenerarVAO(shader, groupList.back(), vertexBuffer, indexBuffer);
				}

				groupList.emplace_back();
				file >> groupList.back().name;
			}
			else if (token == "usemtl")
			{
				if (!groupList.back().meshes.empty())
				{
					groupList.back().meshes.back().size = indexBuffer.size() - groupList.back().meshes.back().offset;
				}

				groupList.back().meshes.emplace_back();
				groupList.back().meshes.back().offset = indexBuffer.size();

				file >> groupList.back().meshes.back().materialName;
			}
			else if (token == "v")
			{
				vec3 position;
				file >> position.x >> position.y >> position.z;

				positions.push_back(position);
			}
			else if (token == "vt")
			{
				vec2 texCoord;
				file >> texCoord.x >> texCoord.y;
				texCoords.push_back(texCoord);
			}
			else if (token == "vn")
			{
				vec3 normal;
				file >> normal.x >> normal.y >> normal.z;
				normals.push_back(normal);
			}
			else if (token[0] == 'f')
			{
				for (unsigned int i = 0; i < 3; i++)
				{
					unsigned int index, positionIndex, texCoordIndex, normalIndex;

					file >> positionIndex;
					file.ignore(INT_MAX, '/');
					file >> texCoordIndex;
					file.ignore(INT_MAX, '/');
					file >> normalIndex;

					Mesh::Vertex vertex;
					vertex.position = positions[positionIndex - 1];
					vertex.texCoord = texCoords[texCoordIndex - 1];
					vertex.normal = normals[normalIndex - 1];

					index = find(vertexBuffer.begin(), vertexBuffer.end(), vertex) - vertexBuffer.begin();

					if (index < vertexBuffer.size())
					{
						indexBuffer.push_back(index);
					}
					else
					{
						vertexBuffer.push_back(vertex);
						indexBuffer.push_back(vertexBuffer.size() - 1);
					}
				}
			}
			file.ignore(INT_MAX, '\n');
		}
		file.close();
	}

	groupList.back().meshes.back().size = indexBuffer.size();

	CalcularTangentes(vertexBuffer, indexBuffer);
	GenerarVAO(shader, groupList.back(), vertexBuffer, indexBuffer);

	return groupList;
}

GLvoid Model::CalcularTangentes(vector <Mesh::Vertex>& vertices, vector <GLuint>& indices)
{
	vec3* tangents = new vec3[vertices.size()];
	vec3* binormals = new vec3[vertices.size()];

	for (unsigned int i = 0; i < indices.size(); i += 3)
	{
		vec3 vertex0 = vertices[indices[i]].position;
		vec3 vertex1 = vertices[indices[i + 1]].position;
		vec3 vertex2 = vertices[indices[i + 2]].position;

		vec3 normal = glm::cross(vertex1 - vertex0, vertex2 - vertex0);

		vec3 deltaPos;

		if (vertex0.x == vertex1.x && vertex0.y == vertex1.y && vertex0.z == vertex1.z)
		{
			deltaPos = vertex2 - vertex0;
		}
		else
		{
			deltaPos = vertex1 - vertex0;
		}

		vec2 uv0 = vertices[indices[i]].texCoord;
		vec2 uv1 = vertices[indices[i + 1]].texCoord;
		vec2 uv2 = vertices[indices[i + 2]].texCoord;

		vec2 deltaUV1 = uv1 - uv0;
		vec2 deltaUV2 = uv2 - uv0;

		vec3 tan;
		vec3 bin;

		tan = deltaPos / 1.0f;
		tan = tan - (glm::dot(normal, tan) * normal);
		tan = glm::normalize(tan);

		bin = glm::cross(tan, normal);
		bin = glm::normalize(bin);

		tangents[indices[i]] = tan;
		tangents[indices[i + 1]] = tan;
		tangents[indices[i + 2]] = tan;

		binormals[indices[i]] = bin;
		binormals[indices[i + 1]] = bin;
		binormals[indices[i + 2]] = bin;
	}

	for (unsigned int i = 0; i < vertices.size(); i++)
	{
		vertices[i].tangent = tangents[i];
		vertices[i].binormal = binormals[i];
	}
}

vector<Material> Model::CargarMTL(string relativePath, string fileName)
{
	vector<Material> materialList;
	ifstream file;
	file.open(relativePath + "/" + fileName);

	if (file.is_open())
	{
		while (file.good())
		{
			string token;
			file >> token;

			if (token == "newmtl")
			{
				materialList.emplace_back();
				file >> materialList.back().name;
			}
			else if (token == "Ka")
			{
				vec3 _ambient;
				file >> _ambient.x >> _ambient.y >> _ambient.z;
				materialList.back().ambient = _ambient;
			}
			else if (token == "Kd")
			{
				vec3 _diffuse;
				file >> _diffuse.x >> _diffuse.y >> _diffuse.z;
				materialList.back().diffuse = _diffuse;
			}
			else if (token == "Ks")
			{
				vec3 _specular;
				file >> _specular.x >> _specular.y >> _specular.z;
				materialList.back().specular = _specular;
			}
			else if (token == "Ns")
			{
				file >> materialList.back().shininess;
			}
			else if (token == "d")
			{
				file >> materialList.back().transparency;
			}
			else if (token == "illum")
			{
				GLuint illumination;
				file >> illumination;
			}
			else if (token == "map_Ka")
			{
				string name;
				file >> name;
				Texture* _texture = new Texture(Bitmap::bitmapFromFile(relativePath + "/" + name));
				materialList.back().ambientMap = _texture;
			}
			else if (token == "map_Kd")
			{
				string name;
				file >> name;
				Texture* _texture = new Texture(Bitmap::bitmapFromFile(relativePath + "/" + name));
				materialList.back().diffuseMap = _texture;
			}
			else if (token == "map_Ks")
			{
				string name;
				file >> name;
				Texture* _texture = new Texture(Bitmap::bitmapFromFile(relativePath + "/" + name));
				materialList.back().specularMap = _texture;
			}
			else if (token == "map_Ns")
			{
				string name;
				file >> name;
				Texture* _texture = new Texture(Bitmap::bitmapFromFile(relativePath + "/" + name));
				materialList.back().shininessMap = _texture;
			}
			else if (token == "map_d")
			{
				string name;
				file >> name;
				Texture* _texture = new Texture(Bitmap::bitmapFromFile(relativePath + "/" + name));
				materialList.back().transparencyMap = _texture;
			}
			else if (token == "map_Bump")
			{
				string name;
				file >> name;

				int foundExtension = name.find(".png");
				foundExtension = foundExtension > 0 ? foundExtension : name.find(".jpg");
				foundExtension = foundExtension > 0 ? foundExtension : name.find(".bmp");
				foundExtension = foundExtension > 0 ? foundExtension : name.find(".tga");

				if (foundExtension <= 0)
				{
					file >> name;
					file >> name;
				}

				Texture* _texture = new Texture(Bitmap::bitmapFromFile(relativePath + "/" + name));
				materialList.back().normalMap = _texture;
			}

			file.ignore(INT_MAX, '\n');
		}

		file.close();
	}

	return materialList;
}

GLvoid Model::GenerarVAO(Shader* program, Group& group, vector<Mesh::Vertex>& vertices, vector <GLuint>& indices)
{
	GLuint VBO, IBO;

	glGenVertexArrays(1, &group.VAO);
	glBindVertexArray(group.VAO);

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), indices.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Mesh::Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(program->attrib("inputPosition"));
	glVertexAttribPointer(program->attrib("inputPosition"), 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), (const GLvoid*)0);

	glEnableVertexAttribArray(program->attrib("inputTexCoord"));
	glVertexAttribPointer(program->attrib("inputTexCoord"), 2, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), (const GLvoid*)(sizeof(vec3)));

	glEnableVertexAttribArray(program->attrib("inputNormal"));
	glVertexAttribPointer(program->attrib("inputNormal"), 3, GL_FLOAT, GL_TRUE, sizeof(Mesh::Vertex), (const GLvoid*)(sizeof(vec3) + sizeof(vec2)));

	glEnableVertexAttribArray(program->attrib("inputTangent"));
	glVertexAttribPointer(program->attrib("inputTangent"), 3, GL_FLOAT, GL_TRUE, sizeof(Mesh::Vertex), (const GLvoid*)(sizeof(vec3) + sizeof(vec2) + sizeof(vec3)));

	glEnableVertexAttribArray(program->attrib("inputBinormal"));
	glVertexAttribPointer(program->attrib("inputBinormal"), 3, GL_FLOAT, GL_TRUE, sizeof(Mesh::Vertex), (const GLvoid*)(sizeof(vec3) + sizeof(vec2) + sizeof(vec3) + sizeof(vec3)));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	vertices.clear();
	indices.clear();
}