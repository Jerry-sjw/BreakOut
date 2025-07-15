#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <irrKlang/irrKlang.h>
#include "SpriteRenderer.h"
#include "GameLevel.h"
#include "BallObject .h"
#include "Particle.h"
#include "PowerUp.h"
#include "TextRenderer.h"

enum class GameState
{
	GAME_ACTIVATE,
	GAME_MENU,
	GAME_WIN
};

enum class Direction {
	UP,
	RIGHT,
	DOWN,
	LEFT
};

inline GLboolean ShouldSpawn(GLuint chance)
{
	GLuint random = rand() % chance;
	return random == 0;
}

inline GLboolean IsOtherPowerUpActive(std::vector<std::unique_ptr<PowerUp>>& powerUps, std::string type) //注意一定要传powerUps的引用，智能指针没有拷贝构造函数，不加引用会发生拷贝
{
	for (const std::unique_ptr<PowerUp>& powerUp : powerUps)
	{
		if (powerUp->Activated)
			if (powerUp->Type == type)
				return GL_TRUE;
	}
	return GL_FALSE;
}

typedef std::tuple<GLboolean, Direction, glm::vec2> Collision;

// 初始化参数
const glm::vec2 PLAYER_SIZE(100, 20);
const GLfloat PLAYER_VELOCITY(500.0f);
const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);
const GLfloat BALL_RADIUS = 12.5f;

class Game 
{
public:
	GameState State;
	GLboolean Keys[1024];
	GLboolean KeysProcessed[1024];
	GLuint Width, Height;
	GLuint Lives;
	std::vector<std::unique_ptr<BallObject>> BallLives;

	std::vector<GameLevel> GameLevels;
	GLuint GameLevel_index;
	GLuint shake = 0;
	GLfloat shake_time = 0.05f;
	std::vector<std::unique_ptr<PowerUp>> PowerUps;

	Game(GLuint width, GLuint height);

	~Game();

	void Init();
	void ProcessInput(GLfloat dt);
	void Update(GLfloat dt);
	void Render();

	Direction VectorDirection(glm::vec2 target);
	GLboolean CheckCollision(GameObject* one, PowerUp* two);
	Collision  CheckCollision(GameObject* one, BallObject* ball);
	void DoCollision();

	void SpawnPowerUps(GameObject& block);
	void UpdatePowerUps(GLfloat dt);
	void ActivatePowerUp(PowerUp* powerUp);

	void ResetLevel();
	void ResetPlayer();

private:



};