#include "Particle.h"
#include <iostream>

ParticleGenerator::ParticleGenerator(Shader shader, Texture texture, GLuint amount)
    : shader(shader), texture(texture), amount(amount)
{
    this->init();
}

//改了算法！
void ParticleGenerator::Update(GLfloat dt, GameObject& object, GLuint unused, glm::vec2 offset)
{
    // 1. 粒子生成（基于时间累积）
    particleAccumulator += particlesPerSecond * dt;
    while (particleAccumulator >= 1.0f) {
        int unusedParticle = this->firstUnusedParticle();
        this->respawnParticle(this->particles[unusedParticle], object, offset);
        particleAccumulator -= 1.0f;
    }

    // 2. 粒子更新
    for (GLuint i = 0; i < this->amount; ++i)
    {
        Particle& p = this->particles[i];
        p.Life -= dt;
        if (p.Life > 0.0f)
        {
            p.Position += p.Velocity * dt;
            p.Color.a -= dt * 2.5f;
        }
    }
}

//渲染粒子
void ParticleGenerator::Draw()
{
    //切换混合模式
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    this->shader.Use();
    for (Particle& particle : this->particles)
    {
        if (particle.Life > 0.0f)
        {
            this->shader.SetVector2f("offset", particle.Position);
            this->shader.SetVector4f("color", particle.Color);
            this->texture.Bind();
            glBindVertexArray(this->VAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindVertexArray(0);
        }
    }
    // 还原混合模式
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

//粒子生成器初始化
void ParticleGenerator::init()
{
    GLuint VBO;
    GLfloat particle_quad[] = {
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(this->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
    glBindVertexArray(0);

    //生成粒子
    for (GLuint i = 0; i < this->amount; ++i)
        this->particles.push_back(Particle());
}

// 找到第一个消散的粒子索引
GLuint ParticleGenerator::firstUnusedParticle()
{
    for (GLuint i = lastUsedParticle; i < this->amount; ++i) {
        if (this->particles[i].Life <= 0.0f) {
            lastUsedParticle = i;
            return i;
        }
    }

    for (GLuint i = 0; i < lastUsedParticle; ++i) {
        if (this->particles[i].Life <= 0.0f) {
            lastUsedParticle = i;
            return i;
        }
    }
   
    lastUsedParticle = 0;
    return 0;
}

void ParticleGenerator::respawnParticle(Particle& particle, GameObject& object, glm::vec2 offset)
{
    GLfloat random = ((rand() % 100) - 50) / 10.0f;
    GLfloat rColor = 0.5 + ((rand() % 100) / 100.0f);
    particle.Position = object.Position + glm::vec2(0.5f) * object.Size + random ;
    particle.Color = glm::vec4(rColor, rColor, rColor, 1.0f);
    particle.Life = 1.0f;
    particle.Velocity = object.Velocity * 0.1f;
}