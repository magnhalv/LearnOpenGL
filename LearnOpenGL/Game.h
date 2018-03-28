#pragma once

#include "ResourceMananger.h"
#include "SpriteRenderer.h"
#include "GameLevel.h"
#include "BallObject.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

enum GameState {
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN
};

class Game
{
public:
	// Game state
	GameState  State;
	GLboolean  Keys[1024];
	GLuint	   Width, Height;
	std::vector<GameLevel> Levels;
	GLuint Level;

	SpriteRenderer *Renderer;
	GameObject *Player;

	const glm::vec2 PLAYER_SIZE;
	const GLfloat PLAYER_VELOCITY;
	
	const glm::vec2 INITIAL_BALL_VELOCITY;
	const GLfloat BALL_RADIUS = 12.5f;
	BallObject *Ball;


	
	Game(GLuint width, GLuint height);
	~Game();	
	void Init();
	
	// Game loop
	void DoCollisions();
	void ProcessInput(GLfloat dt);
	void Update(GLfloat dt);
	void Render();
private:

	GLboolean isCollision(GameObject &one, GameObject &two);
};