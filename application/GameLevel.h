#pragma once 
#include <glad/glad.h>
#include "GameObject.h"
#include <vector>
#include "SpriteRenderer.h"

class GameLevel
{
public:
	GameLevel();

	std::vector<GameObject> Bricks;

	void Load(GLchar* file, GLuint Width, GLuint Height);
	void Draw(SpriteRenderer* renderer);

	//���ڼ��ؿ��Ƿ�ͨ��
	GLboolean IsCompleted();
private:
	void Init(std::vector<std::vector<GLuint>> tileData, GLuint levelWidth, GLuint levelHeight);
};