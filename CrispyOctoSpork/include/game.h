#pragma once

#include <SDL.h>
#include <vector>
#include <string>

#include "scene.h"

class Game {
public:
	static void Init(std::string windowTitle, int width, int height);
	static void AddScene(Scene scene);
	static void SetActiveScene(std::string sceneName);

	void enabledVerticalSync();
	void disableVerticalSync();
	void showFrameRateCounter();
	void hideFrameRateCounter();
	void enabledFullscreen();
	void disableFullscreen();
private:
	static SDL_Window* window;
	static SDL_Renderer* renderer;

	static std::vector <Scene> scenes;
	static Scene* activeScene;
	static bool gameRunning;

	static bool verticalSyncEnabled;
	static bool frameRateCounterEnabled;
	static bool fullscreenEnabled;

	static void StartGameLoop();
	static void GameLoop();
};