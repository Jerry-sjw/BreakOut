#pragma once

#include <glad/glad.h>
#include "loader/shader.h"
#include "loader/Texture.h"


class SpriteRenderer 
{
public:
	SpriteRenderer(Shader& shader);
	~SpriteRenderer();

	void DrawSprite(Texture& texture, glm::vec2 position, glm::vec2 size = glm::vec2(10, 10), GLfloat rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f));

private:
	Shader shader;
	GLuint quadVAO;
	void initRenderDate();

};