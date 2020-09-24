#include "crispyOctoSporkEngine.h"
using namespace CrispyOctoSpork;

class Player : public Entity
{
public:
	Player(float width, float height, float x, float y, Texture* texture) : Entity(width, height, x, y, texture) {}
};

class SampleGame : public Engine
{
public:
	bool OnCreate() override
	{
		playerTexture = Texture(renderer);
		playerTexture.LoadTextureFromFile(renderer, "assets/ball.png");
		player =  Player(32, 32, screenWidth / 2 - 32, screenHeight / 2 - 32, &playerTexture);
		return true;
	}

	bool OnUpdate(float deltaTime) override
	{
		player.OnUpdate(deltaTime);
		player.OnRender(deltaTime);

		return true;
	}
private:
	Entity player;
	Texture playerTexture;
};

int main(int argc, char* argv[])
{
	SampleGame game;

	if (game.Create("SampleGame", 1280, 720, true, false))
	{
		game.Start();
	}

	return 0;
}