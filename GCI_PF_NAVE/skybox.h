#pragma once

#ifndef _SKYBOX_
#define _SKYBOX_

#include <vector>
#include <string>
#include <glm.hpp>
#include <stb_image_aug.h>
#include "Geometria.h"
#include "includes_opengl.h"
#include "Camera.h"

using namespace std;
using namespace glm;

class Skybox
    : public Geometria
{

private:

    unsigned int cubeMapTextureID;      // dia
    unsigned int cubeMapTexture2ID;     // tarde
    unsigned int cubeMapTexture3ID;     // noche

    float blendFactor = 1.0f;

public:

    enum Tiempo
    {
        Dia,
        Tarde,
        Noche
    };

	Skybox(vector<string> rutasTexturasDia, vector<string> rutasTexturasTarde, vector<string> rutasTexturasNoche, 
        string rutaVertShader, string rutaFragShader)
        : Geometria(rutaVertShader.c_str(), rutaFragShader.c_str())
	{
        unsigned int textureID;

        LoadBox();
        cubeMapTextureID = LoadTextureCubemap(rutasTexturasDia);
        cubeMapTexture2ID = LoadTextureCubemap(rutasTexturasTarde);
        cubeMapTexture3ID = LoadTextureCubemap(rutasTexturasNoche);
	}

	~Skybox()
	{
	}

    void Draw(mat4 view, mat4 projection, vec3 skyColor, float time, bool toNight, float deltaTime)
    {
        glDepthFunc(GL_LEQUAL);
        _Shader->use();

        view = glm::mat4(glm::mat3(view));

        _Shader->setUniform("transform", GetTransformMatrix());
        _Shader->setUniform("view", view);
        _Shader->setUniform("projection", projection);
        _Shader->setUniform("sky", 0);
        _Shader->setUniform("sky2", 1);
        _Shader->setUniform("sky3", 2);
        _Shader->setUniform("skyTime", time);
        _Shader->setUniform("toNight", toNight);
        _Shader->setUniform("fogColor", skyColor);

        glBindVertexArray(VAO);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTextureID);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTexture2ID);

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTexture3ID);

        glDrawArrays(GL_TRIANGLES, 0, 36);

        glBindVertexArray(0);

        _Shader->stopUsing();
        glDepthFunc(GL_LESS);
    }

private:

	void LoadBox()
	{
        float skyboxVertices[] = {

            // positions          
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            -1.0f,  1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f,  1.0f
        };

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	}

	int LoadTextureCubemap(vector<string> rutasTexturas)
	{
		unsigned int textureID;

		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

		for (int i = 0; i < rutasTexturas.size(); i++)
		{
            int width, height, nrChannels;
            unsigned char* imageData = stbi_load(rutasTexturas[i].c_str(), &width, &height, &nrChannels, 0);

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);

            stbi_image_free(imageData);
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		return textureID;
	}

};

#endif // !_SKYBOX_

