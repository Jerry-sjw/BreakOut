#pragma once 

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "loader/shader.h"
#include "loader/Texture.h"

class PostProcessor {
public:
	Shader PostProcessingShader;
	Texture Texture;
	GLuint Width, Height;
	GLboolean Confuse, Chaos, Shake;

	PostProcessor(Shader shader, GLuint width, GLuint height);
	void BeginRender();
	void EndRender();
	void Render(GLfloat time);


private:
	GLuint MSFBO, FBO; 
	GLuint RBO; 
	GLuint VAO;
	void Init();
};