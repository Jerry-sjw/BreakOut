/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#ifndef PARTICLE_GENERATOR_H
#define PARTICLE_GENERATOR_H
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "loader/shader.h"
#include "loader/Texture.h"
#include "GameObject.h"


// Represents a single particle and its state
struct Particle {
    glm::vec2 Position, Velocity;
    glm::vec4 Color;
    GLfloat Life;

    Particle() : Position(0.0f), Velocity(0.0f), Color(1.0f), Life(0.0f) { }
};


// ParticleGenerator acts as a container for rendering a large number of 
// particles by repeatedly spawning and updating particles and killing 
// them after a given amount of time.
class ParticleGenerator
{
public:
    ParticleGenerator(Shader shader, Texture texture, GLuint amount);
    void Update(GLfloat dt, GameObject& object, GLuint newParticles, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
    void Draw();
private:
    std::vector<Particle> particles;
    GLuint amount;
    Shader shader;
    Texture texture;
    GLuint VAO;

    float particleAccumulator = 0.0f;
    float particlesPerSecond = 400.0f; // 每秒生成粒子数，越大越亮
    GLuint lastUsedParticle = 0;

    void init();
    GLuint firstUnusedParticle();
    void respawnParticle(Particle& particle, GameObject& object, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
};

#endif