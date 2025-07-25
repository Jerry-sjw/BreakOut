#pragma once
#include "GameObject.h"

const glm::vec2 SIZE(60, 20);
const glm::vec2 VELOCITY(0.0f, 150.0f);

class PowerUp :public GameObject //显示继承才能访问父类属性
{
public:
	std::string Type;
	GLfloat Duration;
	GLboolean Activated;

	PowerUp(std::string type, glm::vec3 color, GLfloat duration, glm::vec2 position, Texture texture)
		:GameObject(position, SIZE, texture, color, VELOCITY), Type(type), Duration(duration), Activated() {

	}
};

