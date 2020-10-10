#include "crispyOctoSporkEngine.h"
#include "sdlui.h"

using namespace CrispyOctoSpork;

/// <summary>
/// Sample game. Derivies from the base CrispyOctoSpork::Engine.
/// </summary>
class SampleGame : public Engine
{
public:
	Texture* texture;
	SDLUI_Control_Window* uiWindow;
	SDLUI_Control_Button* button;
	SDLUI_Control_Text* slider_text;
	SDLUI_Control_SliderInt* slider;
	Texture* particleTexture;
	ParticleEmitter* particleEmitter;

	/// <summary>
	/// OnCreate gets called once at the begining of the program once SDL has been initilized.
	/// Use this for loading textures, entities, etc.
	/// </summary>
	/// <returns>Returns a boolean indicating success.</returns>
	bool OnCreate() override
	{
		SDLUI_Init(renderer, window);
		uiWindow = SDLUI_CreateWindow(20, 20, 200, 400, "Settings");
		button = SDLUI_CreateButton(uiWindow, 10, 40, "Toggle");

		slider_text = SDLUI_CreateText(uiWindow, 10, 100, "Particles Per Second");
		slider = SDLUI_CreateSliderInt(uiWindow, 10, 120, 100, 10000, 100);

		texture = new Texture(renderer);
		particleTexture = new Texture(renderer);
		texture->LoadTextureFromFile("../assets/ball.png");
		particleTexture->LoadTextureFromFile("../assets/particle.png");

		//AddEntity(new Sprite(screenWidth / 2 - 32 - 64, screenHeight / 2 + 64, 32, 32, &texture, renderer));
		//AddEntity(new Circle(screenWidth / 2 - 16, screenHeight / 2 + 80, 16, COLOR_BLUE, renderer));
		//AddEntity(new Rectangle(screenWidth / 2 + 32, screenHeight / 2 + 64, 32, 32, COLOR_GREEN, renderer));

		particleEmitter = new ParticleEmitter((float) (screenWidth / 2 - 16), (float) (screenHeight / 2 - 16), 1500, particleTexture, 0.3, 100);

		return true;
	}

	bool OnEvent(SDL_Event event) override
	{
		SDLUI_EventHandler(event);
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
		particleEmitter->OnUpdate(deltaTime);
		SDLUI_WindowHandler();
		SDLUI_Window(uiWindow);
		
		if (SDLUI_Button(button)) 
		{ 
			particleEmitter->active = !particleEmitter->active;
			particleEmitter->startOfSecond = SDL_GetTicks();
			particleEmitter->particlesCreatedThisSecond = 0;
		}

		SDLUI_Text(slider_text);

		if (SDLUI_SliderInt(slider)) 
		{ 
			particleEmitter->newParticlesPerSecond = slider->value;
			particleEmitter->startOfSecond = SDL_GetTicks();
			particleEmitter->particlesCreatedThisSecond = 0;
		}

		SDLUI_Render();
		return true;
	}

	bool OnDestroy() override
	{
		Engine::OnDestroy();
		delete particleEmitter;
		delete texture;
		delete particleTexture;
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
	if (game.Create("SampleGame", 1280, 720, false, false))
	{
		game.Start();
	}

	return 0;
}