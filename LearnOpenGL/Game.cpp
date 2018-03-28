#include "game.h"
//#include "resource_manager.h"
Game::Game(GLuint width, GLuint height)
	: State(GAME_ACTIVE), Keys(), Width(width), Height(height), PLAYER_SIZE(100, 20), PLAYER_VELOCITY(500.0f), 
	INITIAL_BALL_VELOCITY(100.0f, -350.0f)
{}

Game::~Game()
{
	delete Renderer;
	delete Player;
}

void Game::Init()
{
	// Load shaders
	ResourceManager::LoadShader("shaders/sprite.vs", "shaders/sprite.frag", nullptr, "sprite");
	// Configure shaders
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(Width), static_cast<GLfloat>(Height), 0.0f, -1.0f, 1.0f);
	ResourceManager::GetShader("sprite").Use().SetInteger("sprite", 0);
	ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
	// Load textures
	ResourceManager::LoadTexture("textures/background.jpg", GL_FALSE, "background");
	ResourceManager::LoadTexture("textures/awesomeface.png", GL_TRUE, "face");
	ResourceManager::LoadTexture("textures/block.png", GL_FALSE, "block");
	ResourceManager::LoadTexture("textures/block_solid.png", GL_FALSE, "block_solid");
	ResourceManager::LoadTexture("textures/paddle.png", true, "paddle");
	
	auto spriteShader = ResourceManager::GetShader("sprite");
	Renderer = new SpriteRenderer(spriteShader);
	// Load levels
	GameLevel one; one.Load("resources/levels/one.lvl", Width, Height * 0.5);
	GameLevel two; two.Load("resources/levels/two.lvl", Width, Height * 0.5);
	GameLevel three; three.Load("resources/levels/three.lvl", Width, Height * 0.5);
	GameLevel four; four.Load("resources/levels/four.lvl", Width, Height * 0.5);
	Levels.push_back(one);
	Levels.push_back(two);
	Levels.push_back(three);
	Levels.push_back(four);
	Level = 0;

	glm::vec2 playerPos = glm::vec2(
		Width / 2 - PLAYER_SIZE.x / 2,
		Height - PLAYER_SIZE.y
	);
	Player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("paddle"));

	glm::vec2 ballPos = playerPos + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -BALL_RADIUS * 2);
	Ball = new BallObject(ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY,
		ResourceManager::GetTexture("face"));

}

void Game::Update(GLfloat dt)
{
	Ball->Move(dt, Width);
	DoCollisions();
}


void Game::DoCollisions()
{
	for (GameObject &box : Levels[Level].Bricks)
	{
		if (!box.Destroyed)
		{
			if (isCollision(*Ball, box))
			{
				if (!box.IsSolid)
					box.Destroyed = GL_TRUE;
			}
		}
	}
}

void Game::ProcessInput(GLfloat dt)
{
	if (State == GAME_ACTIVE)
	{
		GLfloat velocity = PLAYER_VELOCITY * dt;

		if (Keys[GLFW_KEY_A])
		{
			if (Player->Position.x >= 0) {				
				Player->Position.x -= velocity;

				if (Ball->Stuck)
					Ball->Position.x -= velocity;
			}				
		}
		if (Keys[GLFW_KEY_D])
		{
			if (Player->Position.x <= Width - Player->Size.x) {
				Player->Position.x += velocity;

				if (Ball->Stuck)
					Ball->Position.x += velocity;
			}
		}

		if (Keys[GLFW_KEY_SPACE])
			Ball->Stuck = false;
	}
}

void Game::Render()
{
	if (State == GAME_ACTIVE)
	{
		// Draw background
		auto background = ResourceManager::GetTexture("background");
		Renderer->DrawSprite(background, glm::vec2(0, 0), glm::vec2(Width, Height), 0.0f);
		// Draw level
		Levels[Level].Draw(*Renderer);

		Player->Draw(*Renderer);
		Ball->Draw(*Renderer);
	}
}

GLboolean Game::isCollision(GameObject &one, GameObject &two) {
	bool collisionX = one.Position.x + one.Size.x >= two.Position.x &&
		two.Position.x + two.Size.x >= one.Position.x;

	bool collisionY = one.Position.y + one.Size.y >= two.Position.y &&
		two.Position.y + two.Size.y >= one.Position.y;

	return collisionX && collisionY;
}