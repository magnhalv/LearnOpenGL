#pragma once

#include "ResourceMananger.h"
#include "SpriteRenderer.h"
#include "GameLevel.h"
#include "BallObject.h"
#include "ParticaleGenerator.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <tuple>

enum GameState {
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN
};

enum Direction {
	UP,
	RIGHT,
	DOWN,
	LEFT
};

typedef std::tuple<GLboolean, Direction, glm::vec2> Collision;

class Game
{
public:
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

	ParticleGenerator *Particles;

	
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
	Collision isCollision(BallObject &one, GameObject &two);	
	Direction VectorDirection(glm::vec2 target);
	void resetLevel();
	void resetPlayer();
};