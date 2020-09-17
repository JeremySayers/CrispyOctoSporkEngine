#pragma once
#include <SDL.h>

class Entity 
{
public:
	Entity();
	virtual void Render(SDL_Renderer* renderer, int deltaTime);
	virtual void Update(int deltaTime);
};
