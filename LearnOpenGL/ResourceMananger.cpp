#include "ResourceMananger.h"
#include "Shader.h"

#include <iostream>
#include <sstream>
#include <fstream>

#include <stb_image\stb_image.h>

// Instantiate static variables
std::map<std::string, Texture2D>    ResourceManager::Textures;
std::map<std::string, Shader>       ResourceManager::Shaders;


Shader ResourceManager::LoadShader(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile, std::string name)
{
	Shaders[name] = _loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
	return Shaders[name];
}

Shader ResourceManager::GetShader(std::string name)
{
	return Shaders[name];
}

Texture2D ResourceManager::LoadTexture(const GLchar *file, GLboolean hasAlpha, std::string name)
{
	Textures[name] = _loadTextureFromFile(file, hasAlpha);
	return Textures[name];
}

Texture2D ResourceManager::GetTexture(std::string name)
{
	return Textures[name];
}

void ResourceManager::Clear()
{
	// (Properly) delete all shaders	
	for (auto iter : Shaders)
		glDeleteProgram(iter.second.ID);
	// (Properly) delete all textures
	for (auto iter : Textures)
		glDeleteTextures(1, &iter.second.ID);
}

Shader ResourceManager::_loadShaderFromFile(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile)
{
	std::string vertexCode;
	std::string fragmentCode;
	std::string geometryCode;
	try
	{
		std::ifstream vertexShaderFile(vShaderFile);
		std::ifstream fragmentShaderFile(fShaderFile);
		std::stringstream vShaderStream, fShaderStream;

		vShaderStream << vertexShaderFile.rdbuf();
		fShaderStream << fragmentShaderFile.rdbuf();

		vertexShaderFile.close();
		fragmentShaderFile.close();

		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();

		if (gShaderFile != nullptr)
		{
			std::ifstream geometryShaderFile(gShaderFile);
			std::stringstream gShaderStream;
			gShaderStream << geometryShaderFile.rdbuf();
			geometryShaderFile.close();
			geometryCode = gShaderStream.str();
		}
	}
	catch (std::exception e)
	{
		std::cout << "ERROR::SHADER: Failed to read shader files" << std::endl;
	}
	const GLchar *vShaderCode = vertexCode.c_str();
	const GLchar *fShaderCode = fragmentCode.c_str();
	const GLchar *gShaderCode = geometryCode.c_str();
	
	Shader shader;
	shader.Compile(vShaderCode, fShaderCode, gShaderFile != nullptr ? gShaderCode : nullptr);
	return shader;
}

Texture2D ResourceManager::_loadTextureFromFile(const GLchar *file, GLboolean hasAlpha)
{
	// Create Texture object
	Texture2D texture;
	if (hasAlpha)
	{
		texture.Internal_Format = GL_RGBA;
		texture.Image_Format = GL_RGBA;
	}
	// Load image
	int width, height, nrChannels;
	//stbi_set_flip_vertically_on_load(true);
	unsigned char *image = stbi_load(file, &width, &height, &nrChannels, 0);
	if (image) {
		texture.Generate(width, height, image);
	}	
	else {
		std::cout << "Failed to load texture: " << file << std::endl;
	}
	// And finally free image data
	stbi_image_free(image);
	return texture;
}