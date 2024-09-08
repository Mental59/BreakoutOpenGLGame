#include "Game.h"
#include <iostream>

Game::Game(int width, int height) : mWidth(width), mHeight(height), mKeys(), mState(GAME_ACTIVE)
{

}

void Game::Init()
{

}

void Game::ProcessInput(float dt)
{

}

void Game::Update(float dt)
{

}

void Game::Render()
{

}

Game::~Game()
{
#ifdef _DEBUG
	std::cout << "Game is destroyed" << std::endl;
#endif // _DEBUG
}
