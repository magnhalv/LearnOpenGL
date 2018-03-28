#include "game.h"
//#include "resource_manager.h"

SpriteRenderer *Renderer;

Game::Game(GLuint width, GLuint height)
	: State(GAME_ACTIVE), Keys(), Width(width), Height(height)
{	
}

Game::~Game()
{

}

void Game::Init()
{
	// Load shaders
	ResourceManager::LoadShader("./shaders/vertex.glsl", "./shaders/fragment.glsl", nullptr, "sprite");
	// Configure shaders
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->Width),
		static_cast<GLfloat>(this->Height), 0.0f, -1.0f, 1.0f);
	ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
	ResourceManager::GetShader("sprite").Use().SetMatrix4("projection", projection);
	// Set render-specific controls
	auto spriteShader = ResourceManager::GetShader("sprite");
	Renderer = new SpriteRenderer(spriteShader);
	// Load textures
	ResourceManager::LoadTexture("./textures/awesomeface.png", GL_TRUE, "face");
}

void Game::Update(GLfloat dt)
{

}


void Game::ProcessInput(GLfloat dt)
{

}

void Game::Render()
{
	auto faceTexture = ResourceManager::GetTexture("face");
	Renderer->DrawSprite(faceTexture, glm::vec2(200, 200), glm::vec2(300, 400), 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
}