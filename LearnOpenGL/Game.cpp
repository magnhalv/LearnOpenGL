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
	ResourceManager::LoadShader("shaders/particle.vs", "shaders/particle.frag", nullptr, "particle");
	// Configure shaders
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(Width), static_cast<GLfloat>(Height), 0.0f, -1.0f, 1.0f);
	ResourceManager::GetShader("sprite").Use().SetInteger("sprite", 0);
	ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
	ResourceManager::GetShader("particle").Use().SetInteger("sprite", 0);
	ResourceManager::GetShader("particle").SetMatrix4("projection", projection);
	// Load textures
	ResourceManager::LoadTexture("textures/background.jpg", GL_FALSE, "background");
	ResourceManager::LoadTexture("textures/awesomeface.png", GL_TRUE, "face");
	ResourceManager::LoadTexture("textures/block.png", GL_FALSE, "block");
	ResourceManager::LoadTexture("textures/block_solid.png", GL_FALSE, "block_solid");
	ResourceManager::LoadTexture("textures/paddle.png", GL_TRUE, "paddle");
	ResourceManager::LoadTexture("textures/particle.png", GL_TRUE, "particle");
	
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

	Particles = new ParticleGenerator(
		ResourceManager::GetShader("particle"),
		ResourceManager::GetTexture("particle"),
		500
	);

}

void Game::Update(GLfloat dt)
{
	Ball->Move(dt, Width);
	DoCollisions();
	Particles->Update(dt, *Ball, 2, glm::vec2(Ball->Radius / 2));

	if (Ball->Position.y >= this->Height) 
	{
		resetLevel();
		resetPlayer();
	}
}

void Game::resetLevel() {
	Levels[Level].Reset();
}

void Game::resetPlayer() {
	glm::vec2 playerPos = glm::vec2(
		Width / 2 - PLAYER_SIZE.x / 2,
		Height - PLAYER_SIZE.y
	);

	Player->Position = playerPos;

	glm::vec2 ballPos = playerPos + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -BALL_RADIUS * 2);
	Ball->Position = ballPos;
	Ball->Stuck = GL_TRUE;
}


void Game::DoCollisions()
{
	for (GameObject &box : Levels[Level].Bricks)
	{
		if (!box.Destroyed)
		{
			Collision collision = isCollision(*Ball, box);
			if (std::get<0>(collision)) 
			{
				if (!box.IsSolid)
					box.Destroyed = GL_TRUE;

				Direction dir = std::get<1>(collision);
				glm::vec2 diff_vector = std::get<2>(collision);
				if (dir == LEFT || dir == RIGHT) 
				{
					Ball->Velocity.x = -Ball->Velocity.x;
					// Relocate
					GLfloat penetration = Ball->Radius - std::abs(diff_vector.x);
					if (dir == LEFT)
						Ball->Position.x += penetration;
					else
						Ball->Position.x -= penetration;
				}
				else
				{
					Ball->Velocity.y = -Ball->Velocity.y;
					
					//Relocate
					GLfloat penetration = Ball->Radius - std::abs(diff_vector.y);
					if (dir == UP)
						Ball->Position.y -= penetration;
					else
						Ball->Position.y += penetration;
				}
			}
		}
	}

	Collision result = isCollision(*Ball, *Player);
	if (!Ball->Stuck && std::get<0>(result))
	{
		// Check where it hit the board, and change velocity based on where it hit the board
		GLfloat centerBoard = Player->Position.x + Player->Size.x / 2;
		GLfloat distance = (Ball->Position.x + Ball->Radius) - centerBoard;
		GLfloat percentage = distance / (Player->Size.x / 2);
		// Then move accordingly
		GLfloat strength = 2.0f;
		glm::vec2 oldVelocity = Ball->Velocity;
		Ball->Velocity.x = INITIAL_BALL_VELOCITY.x * percentage * strength;
		Ball->Velocity.y = -1 * abs(Ball->Velocity.y); //To avoid the ball getting stuck inside the peddle
		Ball->Velocity = glm::normalize(Ball->Velocity) * glm::length(oldVelocity);
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
		auto background = ResourceManager::GetTexture("background");
		Renderer->DrawSprite(background, glm::vec2(0, 0), glm::vec2(Width, Height), 0.0f);

		Levels[Level].Draw(*Renderer);

		Player->Draw(*Renderer);
		Particles->Draw();
		Ball->Draw(*Renderer);		
	}
}

// AABB - AABB collision
GLboolean Game::isCollision(GameObject &one, GameObject &two)
{
	bool collisionX = one.Position.x + one.Size.x >= two.Position.x &&
		two.Position.x + two.Size.x >= one.Position.x;
	bool collisionY = one.Position.y + one.Size.y >= two.Position.y &&
		two.Position.y + two.Size.y >= one.Position.y;
	return collisionX && collisionY;
}

// AABB - Circle collision
Collision Game::isCollision(BallObject &one, GameObject &two) {
	glm::vec2 ballCenter(one.Position + one.Radius);

	glm::vec2 aabb_half_extents(two.Size.x / 2, two.Size.y / 2);
	glm::vec2 aabb_center(two.Position.x + aabb_half_extents.x, two.Position.y + aabb_half_extents.y);

	glm::vec2 difference = ballCenter - aabb_center;
	glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);

	glm::vec2 aabb_closest = aabb_center + clamped;

	difference = aabb_closest - ballCenter;
	
	if (glm::length(difference) <= one.Radius)
		return std::make_tuple(GL_TRUE, VectorDirection(difference), difference);
	else
		return std::make_tuple(GL_FALSE, UP, glm::vec2(0, 0));
}

Direction Game::VectorDirection(glm::vec2 target)
{
	glm::vec2 compass[] = {
		glm::vec2(0.0f, 1.0f),	// up
		glm::vec2(1.0f, 0.0f),	// right
		glm::vec2(0.0f, -1.0f),	// down
		glm::vec2(-1.0f, 0.0f)	// left
	};
	GLfloat max = 0.0f;
	GLuint best_match = -1;
	for (GLuint i = 0; i < 4; i++)
	{
		GLfloat dot_product = glm::dot(glm::normalize(target), compass[i]);
		if (dot_product > max)
		{
			max = dot_product;
			best_match = i;
		}
	}
	return (Direction)best_match;
}