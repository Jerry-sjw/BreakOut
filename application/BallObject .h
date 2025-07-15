#pragma once
#include "GameObject.h"


class BallObject :public GameObject
{
public:
	GLfloat Radius;
	GLboolean Stuck;
	GLboolean Sticky, PassThrough;
	GLboolean IsCollidingWithPlayer; //挡板和球碰撞标志变量，用来解决角落碰撞时，球在挡板内部导致反复播放碰撞音乐，形成卡顿

	BallObject();
	BallObject(glm::vec2 pos, GLfloat radius, glm::vec2 velocity, Texture sprite);
	~BallObject();

	glm::vec2 move(GLfloat dt, GLuint window_width);
	
	void Reset(glm::vec2 position, glm::vec2 velocity);
};