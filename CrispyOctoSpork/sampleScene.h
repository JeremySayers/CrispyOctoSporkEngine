#pragma once
#include "scene.h"
#include "entity.h"
#include <vector>

class SampleScene : public Scene
{
public:
	SampleScene();
	void Load(SDL_Renderer* renderer);
	virtual void Render(SDL_Renderer* renderer, int deltaTime);
	virtual void Update(int deltaTime);
	void Unload();
private:
	std::vector <Entity> entities;
};