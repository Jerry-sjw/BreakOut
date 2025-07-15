#include "GameLevel.h"
#include <fstream>
#include <sstream>
#include <loader/Loader.h>

GameLevel::GameLevel(){}

void GameLevel::Load(GLchar* file, GLuint Width, GLuint Height)
{
	this->Bricks.clear();

	std::vector<std::vector<GLuint>> tileData;
	std::string line;
	GLuint tileCode;
	std::ifstream fstream(file);
	if (fstream) {
		while (std::getline(fstream, line)) {
			std::istringstream sstream(line);
			std::vector<GLuint> row;
			while (sstream >> tileCode) {
				row.push_back(tileCode);
			}
			tileData.push_back(row);
		}
		if (tileData.size() > 0) {
			Init(tileData, Width, Height);
		}
	}
}

void GameLevel::Init(std::vector<std::vector<GLuint>> tileData, GLuint levelWidth, GLuint levelHeight)
{	
	GLuint col_num = tileData[0].size();
	GLuint row_num = tileData.size();
	GLuint X_size = levelWidth / col_num;
	GLuint Y_size = levelHeight / row_num;
	for (GLuint x = 0; x < col_num; x++) {
		for (GLuint y = 0; y < row_num; y++) {
			if (tileData[y][x] == 1) {
				glm::vec2 pos(X_size * x, Y_size * y);
				glm::vec2 size(X_size, Y_size);

				GameObject obj(pos, size, ResourceManager::GetTexture("block_solid"), glm::vec3(0.8f, 0.8f, 0.7f));
				obj.IsSolid = true;
				this->Bricks.push_back(obj);
			}
			else
			{
				glm::vec2 pos(X_size * x, Y_size * y);
				glm::vec2 size(X_size, Y_size);

				//根据不同的值赋给砖块不同的颜色
				glm::vec3 color = glm::vec3(1.0f); 
				if (tileData[y][x] == 2)
					color = glm::vec3(0.2f, 0.6f, 1.0f);
				else if (tileData[y][x] == 3)
					color = glm::vec3(0.0f, 0.7f, 0.0f);
				else if (tileData[y][x] == 4)
					color = glm::vec3(0.8f, 0.8f, 0.4f);
				else if (tileData[y][x] == 5)
					color = glm::vec3(1.0f, 0.5f, 0.0f);

				GameObject obj(pos, size, ResourceManager::GetTexture("block"), color);
				obj.IsSolid = false;
				this->Bricks.push_back(obj);
			}
		}
	}
}

GLboolean GameLevel::IsCompleted()
{
	for (auto x : this->Bricks) {
		if (x.IsSolid == false && x.Destroyed == false) return GL_FALSE;
	}

	return GL_TRUE;
}

void GameLevel::Draw(SpriteRenderer* renderer) {
	for (auto x : this->Bricks) {
		if (x.Destroyed == false) x.Draw(renderer);
	}
}