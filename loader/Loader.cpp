#include "Loader.h"

#include <iostream>
#include <sstream>
#include <fstream>

#include <STB/stb_image.h>

//Ϊ��̬��Ա����Shaders��Textures����ռ�
std::map<std::string, Shader> ResourceManager::Shaders;
std::map<std::string, Texture> ResourceManager::Textures;

Shader ResourceManager::LoadShader(const GLchar* vShaderFile, const GLchar* fShaderFile, const GLchar* gShaderFile, std::string name) {
	Shaders[name] = LoadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
	return Shaders[name];
}


Texture ResourceManager::LoadTexture(const GLchar* file, GLboolean alpha, std::string name) {
	Textures[name] = LoadTextureFromFile(file, alpha);
	return Textures[name];
}

Shader ResourceManager::LoadShaderFromFile(const GLchar* vShaderFile, const GLchar* fShaderFile, const GLchar* gShaderFile) {
	std::string vertexCode;
	std::string fragmentCode;
	std::string geometryCode;
	try
	{
		//1.���������ļ������ڶ�ȡshader
		std::ifstream vertexShaderFile(vShaderFile);
		std::ifstream fragmentShaderFile(fShaderFile);
		std::stringstream vShaderStream, fShaderStream;
		//2.��ȡ�ļ����ݣ���ת��Ϊstr
		vShaderStream << vertexShaderFile.rdbuf();
		fShaderStream << fragmentShaderFile.rdbuf();
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

	}catch (std::exception e){
		std::cout << "ERROR:Filed to read shader files." << std::endl;
	}
	//3.ת��Ϊc���Է����ַ���
	const GLchar* vShaderCode = vertexCode.c_str();
	const GLchar* fShaderCode = fragmentCode.c_str();
	const GLchar* gShaderCode = geometryCode.c_str();

	//4.����������shader
	Shader shader;
	shader.Compile(vShaderCode, fShaderCode, gShaderFile != nullptr ? gShaderCode : nullptr);
	return shader;
}

Texture ResourceManager::LoadTextureFromFile(const GLchar* file, GLboolean alpha) {
	//1.����ͼƬ
	int width, height, channel;
	unsigned char* data = stbi_load(file, &width, &height, &channel, 0);
	//2.��������
	Texture texture;
	if (alpha) {
		texture.Internal_Format = GL_RGBA;
		texture.Image_Format = GL_RGBA;
	}
	if (data) {
		texture.Generate(width, height, data);
	}
	else {
		std::cerr << "Failed to load texture: " << stbi_failure_reason() << std::endl;

	}
	stbi_image_free(data);
	return texture;
}

Shader ResourceManager::GetShader(std::string name) {
	return Shaders[name];
}

Texture ResourceManager::GetTexture(std::string name) {
	return Textures[name];
}

void ResourceManager::Clear() {
	for (auto iter : Shaders) {
		glDeleteProgram(iter.second.ID);
	}

	for (auto iter : Textures) {
		glDeleteTextures(1, &iter.second.ID);
	}
}