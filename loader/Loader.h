#include <glad/glad.h>
#include "Texture.h"
#include "shader.h"
#include <map>
#include <string>

class ResourceManager
{
public:
	static std::map<std::string, Shader> Shaders;
	static std::map<std::string, Texture> Textures;

	static Shader LoadShader(const GLchar* vShaderFile, const GLchar* fShaderFile, const GLchar* gShaderFile, std::string name);
	static Texture LoadTexture(const GLchar* file, GLboolean alpha, std::string name);

	static Shader GetShader(std::string name);
	static Texture GetTexture(std::string name);

	static void	Clear();

private:
	ResourceManager(){}
	static Shader LoadShaderFromFile(const GLchar* vShaderFile, const GLchar* fShaderFile, const GLchar* gShaderFile = nullptr);
	static Texture LoadTextureFromFile(const GLchar* file, GLboolean alpha);

};