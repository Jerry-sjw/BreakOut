#pragma once
#include "GameObject.h"


class BallObject :public GameObject
{
public:
	GLfloat Radius;
	GLboolean Stuck;
	GLboolean Sticky, PassThrough;
	GLboolean IsCollidingWithPlayer; //���������ײ��־�������������������ײʱ�����ڵ����ڲ����·���������ײ���֣��γɿ���

	BallObject();
	BallObject(glm::vec2 pos, GLfloat radius, glm::vec2 velocity, Texture sprite);
	~BallObject();

	glm::vec2 move(GLfloat dt, GLuint window_width);
	
	void Reset(glm::vec2 position, glm::vec2 velocity);
};