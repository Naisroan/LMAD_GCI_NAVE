#pragma once

#ifndef _SHADERFILE_
#define _SHADERFILE_

#include <Windows.h>
#include <string>
#include <glew.h>
#include <stdexcept>
#include <fstream>
#include <sstream>

#include "funciones.h"

class ShaderFile
{

private:

	GLuint shader = 0;
	unsigned* _refCount = NULL;

	void _retain()
	{
		assert(_refCount);
		*_refCount += 1;
	}

	void _release()
	{
		assert(_refCount && *_refCount > 0);
		*_refCount -= 1;

		if (*_refCount == 0) 
		{
			glDeleteShader(shader); 
			shader = 0;
			delete _refCount; _refCount = NULL;
		}
	}

public:

	GLuint GetShader() const 
	{
		return shader;
	}

	ShaderFile(const std::string& shaderCode, GLenum shaderType)
	{
		shader = glCreateShader(shaderType);

		if (shader == 0)
		{
			Fun::ShowMessage(MSG_CAPTION_ERROR, "Fallo al crear el shader");
			throw std::runtime_error("Fallo al crear el shader");
			PostQuitMessage(0);
		}

		//set the source code
		const char* code = shaderCode.c_str();
		glShaderSource(shader, 1, (const GLchar**)&code, NULL);

		//compile
		glCompileShader(shader);

		//throw exception if compile error occurred
		GLint status;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

		if (status == GL_FALSE) 
		{
			std::string msg("Fallo al compilar el shader:\n");
			GLint infoLogLength;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

			char* strInfoLog = new char[infoLogLength + 1];
			glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);
			msg += strInfoLog;
			delete[] strInfoLog;

			glDeleteShader(shader); shader = 0;

			Fun::ShowMessage(MSG_CAPTION_ERROR, msg);
			throw std::runtime_error(msg);
			PostQuitMessage(0);
		}

		_refCount = new unsigned;
		*_refCount = 1;
	}

	ShaderFile(const ShaderFile& other) :
		shader(other.shader),
		_refCount(other._refCount)
	{
		_retain();
	}

	ShaderFile& operator =(const ShaderFile& other)
	{
		_release();
		shader = other.shader;
		_refCount = other._refCount;
		_retain();
		return *this;
	}

	~ShaderFile()
	{
		if (_refCount)
		{
			_release();
		}
	}

	static ShaderFile LoadShaderFromFile(const std::string& filePath, GLenum shaderType)
	{
		std::ifstream f;

		f.open(filePath.c_str(), std::ios::in | std::ios::binary);

		if (!f.is_open()) 
		{
			Fun::ShowMessage(MSG_CAPTION_ERROR, "Fallo al abrir el archivo del shader");
			throw std::runtime_error(std::string("Fallo al abrir el archivo del shader") + filePath);
			PostQuitMessage(0);
		}

		std::stringstream buffer;
		buffer << f.rdbuf();

		ShaderFile shader(buffer.str(), shaderType);

		return shader;
	}
};

#endif // !_SHADERFILE_
