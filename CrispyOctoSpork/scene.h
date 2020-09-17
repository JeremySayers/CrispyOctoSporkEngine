#pragma once
#include <SDL.h>
#include "game.h"

class Scene {
public:
	Scene();
	Scene(std::string sceneName);
	virtual void Load(SDL_Renderer* renderer);
	virtual void Render(SDL_Renderer* renderer, int deltaTime);
	virtual void Update(int deltaTime);
	virtual void Unload();

	std::string name;
};