#include "BallObject .h"

BallObject::BallObject(){}

BallObject::BallObject(glm::vec2 pos, GLfloat radius, glm::vec2 velocity, Texture sprite)
	:GameObject(pos, glm::vec2(radius*2, radius*2), sprite, glm::vec3(1.0f), velocity), Radius(radius), Stuck(GL_TRUE), Sticky(GL_FALSE),PassThrough(GL_FALSE), IsCollidingWithPlayer(GL_FALSE){

}

BallObject::~BallObject(){}

glm::vec2 BallObject::move(GLfloat dt, GLuint window_width)
{

    //�Ľ��㣺1.�����ٶȷ���ԭλ�ã�ֱ�������ڱ߽硣
    //        2.�ٶȲ���abs����ֹ���ٳ��ֶ�η��������ٶȳ���
    if (!this->Stuck)
    {
        // �ƶ�λ��
        this->Position += this->Velocity * dt;

        // ײ�ұ߽�
        if (this->Position.x + this->Size.x >= window_width)
        {
            this->Velocity.x = -std::abs(this->Velocity.x);
            this->Position.x = window_width - this->Size.x;
        }
        // ײ��߽�
        else if (this->Position.x <= 0.0f)
        {
            this->Velocity.x = std::abs(this->Velocity.x);
            this->Position.x = 0.0f;
        }

        // ײ�ϱ߽�
        if (this->Position.y <= 0.0f)
        {
            this->Velocity.y = std::abs(this->Velocity.y);
            this->Position.y = 0.0f;
        }

    }

    return this->Position;
}
void BallObject::Reset(glm::vec2 position, glm::vec2 velocity)
{
	this->Position = position;
	this->Velocity = velocity;
	this->Stuck = true;
}