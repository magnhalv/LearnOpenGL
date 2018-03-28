#pragma once

#include <glad\glad.h>
#include "GameObject.h"
#include "SpriteRenderer.h"
#include "ResourceMananger.h"
#include <vector>


class GameLevel
{
public:
	std::vector<GameObject> Bricks;

	GameLevel();
	void Load(const GLchar *file, GLuint levelWidth, GLuint levelHeight);
	void Draw(SpriteRenderer &renderer);
	GLboolean IsCompleted();
private:
	void init(std::vector<std::vector<GLuint>> tileData, GLuint levelWidth, GLuint levelHeight);
};
