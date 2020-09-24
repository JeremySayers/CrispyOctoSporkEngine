#include "crispyOctoSporkEngine.h"

class SampleGame : public CrispyOctoSpork::Engine
{
public:
	SampleGame()
	{}

	bool OnCreate() override
	{
		return true;
	}

	bool OnUpdate(float deltaTime) override
	{
		return true;
	}
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