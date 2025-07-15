#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "loader/Texture.h"
#include "SpriteRenderer.h"


class GameObject
{
public:
	GameObject();
	GameObject(glm::vec2 pos, glm::vec2 size, Texture sprite, glm::vec3 color = glm::vec3(1.0f), glm::vec2 velocity = glm::vec2(0.0f, 0.0f));
	~GameObject();

	glm::vec2 Position, Size, Velocity;
	glm::vec3 Color;
	GLfloat Rotation;
	GLboolean Destroyed;
	GLboolean IsSolid;
	Texture   Sprite;

	virtual void Draw(SpriteRenderer* renderer);
};