#include "BallObject .h"

BallObject::BallObject(){}

BallObject::BallObject(glm::vec2 pos, GLfloat radius, glm::vec2 velocity, Texture sprite)
	:GameObject(pos, glm::vec2(radius*2, radius*2), sprite, glm::vec3(1.0f), velocity), Radius(radius), Stuck(GL_TRUE), Sticky(GL_FALSE),PassThrough(GL_FALSE), IsCollidingWithPlayer(GL_FALSE){

}

BallObject::~BallObject(){}

glm::vec2 BallObject::move(GLfloat dt, GLuint window_width)
{

    //改进点：1.不按速度返回原位置，直接设置在边界。
    //        2.速度采用abs，防止卡顿出现多次反弹导致速度出错。
    if (!this->Stuck)
    {
        // 移动位置
        this->Position += this->Velocity * dt;

        // 撞右边界
        if (this->Position.x + this->Size.x >= window_width)
        {
            this->Velocity.x = -std::abs(this->Velocity.x);
            this->Position.x = window_width - this->Size.x;
        }
        // 撞左边界
        else if (this->Position.x <= 0.0f)
        {
            this->Velocity.x = std::abs(this->Velocity.x);
            this->Position.x = 0.0f;
        }

        // 撞上边界
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