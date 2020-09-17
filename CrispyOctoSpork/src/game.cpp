#include <string>
#include <SDL.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#endif

#include "../include/game.h"

void Game::Init(std::string windowTitle, int width, int height)
{
	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window* window = SDL_CreateWindow(windowTitle.c_str(), 
							SDL_WINDOWPOS_UNDEFINED, 
							SDL_WINDOWPOS_UNDEFINED, 
							width, 
							height, 
							SDL_WINDOW_SHOWN);

	if (window == NULL)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
			"Crispy Octo Spork - Error",
			"Could not create the window!",
			NULL);
	}

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if (renderer == NULL)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
			"Crispy Octo Spork - Error",
			"Could not create the renderer!",
			window);
	}
	
	window = window;
	renderer = renderer;
}

void Game::AddScene(Scene scene)
{
	// scenes needs to have a name so we can set it active.
	if (scene.name == "")
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
			"Crispy Octo Spork - Error",
			"Scene added without a name, you can't do that.",
			window);
	}

	scenes.push_back(scene);

	// if this is the only scene, make it active.
	if (scenes.size() == 1) {
		SetActiveScene(scene.name);
	}
}

void Game::SetActiveScene(std::string sceneName)
{
	// find the scene in our vector based on the passed in name.
	auto scene = std::find_if(scenes.begin(), scenes.end(), [&sceneName](const Scene& obj) {return obj.name == sceneName;});

	if (scene == scenes.end()) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
			"Crispy Octo Spork - Error",
			"A scene with that name doesnt exist!", 
			NULL);
		exit(1);
	}

	// if a scene was already loaded then we need to unload it.
	if (activeScene != NULL) {
		activeScene->Unload();
	}

	// set the active scene and then call its load method.
	activeScene = &*scene;
	activeScene->Load(renderer);

	// if we don't already have a game loop going then start one.
	if (gameRunning == false) {
		gameRunning = true;
		StartGameLoop();
	}
}

void Game::StartGameLoop() {
#ifdef __EMSCRIPTEN__
	emscripten_set_main_loop_arg(GameLoop, NULL, 60, 1);
#else
	while (gameRunning)
	{
		GameLoop();
	}
#endif
}

void Game::GameLoop() {
	activeScene->Update(0);
	activeScene->Render(renderer, 0);
}
