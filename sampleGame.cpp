#include "crispyOctoSporkEngine.h"
using namespace CrispyOctoSpork;

/// <summary>
/// Sample game. Derivies from the base CrispyOctoSpork::Engine.
/// </summary>
class SampleGame : public Engine
{
public:
	/// <summary>
	/// Example texture to use with a sprite.
	/// </summary>
	Texture texture;

	/// <summary>
	/// OnCreate gets called once at the begining of the program once SDL has been initilized.
	/// Use this for loading textures, entities, etc.
	/// </summary>
	/// <returns>Returns a boolean indicating success.</returns>
	bool OnCreate() override
	{
		texture = Texture(renderer);
		texture.LoadTextureFromFile("assets/ball.png");

		AddEntity(new Sprite(screenWidth / 2 - 32 - 64, screenHeight / 2 - 32, 32, 32, &texture, renderer));
		AddEntity(new Circle(screenWidth / 2 - 16, screenHeight / 2 - 16, 16, COLOR_BLUE, renderer));
		AddEntity(new Rectangle(screenWidth / 2 + 32, screenHeight / 2 - 32, 32, 32, COLOR_GREEN, renderer));

		return true;
	}

	/// <summary>
	/// Called once per frame. If you want the engine to render entities that have
	/// added then you need to call Engine::OnUpdate(deltaTime).
	/// </summary>
	/// <param name="deltaTime">The delta time since the last frame. 
	/// Useful for frame independant movement.</param>
	/// <returns>Returns a boolean indicating success.</returns>
	bool OnUpdate(float deltaTime) override
	{
		Engine::OnUpdate(deltaTime);
		return true;
	}
};

/// <summary>
/// The main entry point of your game.
/// </summary>
/// <param name="argc">The number of arguments.</param>
/// <param name="argv">A char array of arguments provided.</param>
/// <returns>Returns an integer indicating exit status.</returns>
int main(int argc, char* argv[])
{
	SampleGame game;

	// Create a new instance of your game. If successful, then start the main loop.
	if (game.Create("SampleGame", 640, 480, true, false))
	{
		game.Start();
	}

	return 0;
}