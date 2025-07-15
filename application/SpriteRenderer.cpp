#include "SpriteRenderer.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

SpriteRenderer::SpriteRenderer(Shader& shader) {
	this->shader = shader;
	this->initRenderDate();
}

void SpriteRenderer::DrawSprite(Texture& texture, glm::vec2 position,
	glm::vec2 size, GLfloat rotate, glm::vec3 color)
{
	// 准备变换
	this->shader.Use();
	glm::mat4 model;

	//先平移，后缩放。则是固定position点缩放
	model = glm::translate(model, glm::vec3(position, 0.0f));

	//model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
	//model = glm::rotate(model, rotate, glm::vec3(0.0f, 0.0f, 1.0f));
	//model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));

	//model = glm::translate(model, glm::vec3(-0.5f * position.x, -0.5f * position.y, 0.0f));

	model = glm::scale(model, glm::vec3(size, 1.0f));

	this->shader.SetMatrix4("model", model);
	this->shader.SetVector3f("spriteColor", color);
	this->shader.SetInteger("image", 0);

	glActiveTexture(GL_TEXTURE0);
	texture.Bind();

	glBindVertexArray(this->quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void SpriteRenderer::initRenderDate()
{
	GLuint VBO;
	GLfloat vertices[] = {
		// 坐标     // 纹理
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
	};

	glGenVertexArrays(1, &this->quadVAO);
	glGenBuffers(1, &VBO);
	//VBO数据填充
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//设置VAO
	glBindVertexArray(this->quadVAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	//解绑数据
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

SpriteRenderer::~SpriteRenderer()
{

}