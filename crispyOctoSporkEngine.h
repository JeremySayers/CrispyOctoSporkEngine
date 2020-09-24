#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <string>
#include <iostream>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#endif

namespace CrispyOctoSpork
{
	/// <summary>
	/// The main class that games should create an instance of.
	/// </summary>
	class Engine
	{
	public:
		/// <summary>
		/// Default constructor.
		/// </summary>
		Engine();

		/// <summary>
		/// Overridable deconstructor.
		/// </summary>
		virtual ~Engine();

		/// <summary>
		/// First thing to call after creating an instance of <see cref="Engine"/>.
		/// </summary>
		/// <param name="name">The name of the game, will be displayed in the title bar.</param>
		/// <param name="width">The width of the screen in pixels.</param>
		/// <param name="height">The height of the screen in pixels.</param>
		/// <param name="vsync">Indicates if the renderer should be created with vsync on.</param>
		/// <param name="fullscreen">Indicates if the renderer should be created with fullscreen set.</param>
		/// <returns>Returns a boolean indicating if creation was successful.</returns>
		bool Create(std::string name, int width, int height, bool vsync, bool fullscreen);

		/// <summary>
		/// Called to start the game engines main game loop.
		/// </summary>
		/// <returns>Returns a boolean indicating if there was an issue.</returns>
		bool Start();

		/// <summary>
		/// Called once at engine startup.
		/// </summary>
		/// <returns>Returns a boolean.</returns>
		virtual bool OnCreate();

		/// <summary>
		/// Called once per frame after the engine is started.
		/// </summary>
		/// <param name="deltaTime">The time delta from the previous frame.</param>
		/// <returns>Returns a boolean indicating if the engine should continue running.</returns>
		virtual bool OnUpdate(float deltaTime);

		/// <summary>
		/// Called before the engine terminates.
		/// </summary>
		/// <returns>Returns a boolean.</returns>
		virtual bool OnDestroy();

	private:
		SDL_Window* window;
		SDL_Renderer* renderer;
		int screenWidth;
		int screenHeight;
		bool isVsyncEnabled;
		bool isFullscreenEnabled;
		std::string name;
		bool isEngineRunning;

		/// <summary>
		/// The main update function that is called once per frame in the engine.
		/// </summary>
		void Update(void* arg);
	};

	Engine::Engine()
	{
		window = NULL;
		renderer = NULL;
		int screenWidth = 0;
		int screenHeight = 0;
		bool isVsyncEnabled = true;
		bool isFullscreenEnabled = false;
		std::string name = "";
		bool isEngineRunning = false;
	}

	Engine::~Engine()
	{}

	/// <summary>
	/// First thing to call after creating an instance of <see cref="Engine"/>.
	/// </summary>
	/// <param name="name">The name of the game, will be displayed in the title bar.</param>
	/// <param name="width">The width of the screen in pixels.</param>
	/// <param name="height">The height of the screen in pixels.</param>
	/// <param name="vsync">Indicates if the renderer should be created with vsync on.</param>
	/// <param name="fullscreen">Indicates if the renderer should be created with fullscreen set.</param>
	/// <returns>Returns a boolean indicating if creation was successful.</returns>
	bool Engine::Create(std::string name, int width, int height, bool vsync, bool fullscreen)
	{
		this->name = name;
		this->screenWidth = width;
		this->screenHeight = height;
		this->isVsyncEnabled = vsync;
		this->isFullscreenEnabled = fullscreen;

		Uint32 windowFlags = SDL_WINDOW_SHOWN;

		if (this->isFullscreenEnabled)
		{
			windowFlags |= SDL_WINDOW_FULLSCREEN;
		}

		this->window = SDL_CreateWindow(this->name.c_str(), 
										SDL_WINDOWPOS_UNDEFINED, 
										SDL_WINDOWPOS_UNDEFINED, 
										this->screenWidth, 
										this->screenHeight, 
										windowFlags);

		if (this->window == NULL) 
		{
			std::cout << "Failed to create window: " << SDL_GetError() << std::endl;
			return false;
		}

		Uint32 rendererFlags = SDL_RENDERER_ACCELERATED;

		if (this->isVsyncEnabled)
		{
			rendererFlags |= SDL_RENDERER_PRESENTVSYNC;
		}

		this->renderer = SDL_CreateRenderer(window, -1, rendererFlags);

		if (renderer == NULL)
		{
			std::cout << "Failed to create renderer: " << SDL_GetError() << std::endl;
			return false;
		}

		int imgFlags = IMG_INIT_PNG;
		if (!(IMG_Init(imgFlags) & imgFlags))
		{
			std::cout << "Failed to create renderer: " << IMG_GetError() << std::endl;
			return false;
		}
	}

	/// <summary>
	/// Called to start the main game loop.
	/// </summary>
	/// <returns>Returns a boolean.</returns>
	bool Engine::Start()
	{
		isEngineRunning = true;

		#ifdef __EMSCRIPTEN__
		emscripten_set_main_loop_arg(Engine::Update, this, -1, 1);
		#else
		while (isEngineRunning)
		{
			Update(this);
		}
		#endif

		return true;
	}

	void Engine::Update(void* arg)
	{
		class Engine* engine = (class Engine*)arg;

		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				engine->isEngineRunning = false;
				break;
			}
		}

		SDL_SetRenderDrawColor(engine->renderer, 135, 206, 235, 255);
		SDL_RenderClear(engine->renderer);

		SDL_RenderPresent(engine->renderer);
	}

	bool Engine::OnCreate()
	{
		return true;
	}

	bool Engine::OnUpdate(float deltaTime)
	{
		return true;
	}

	bool Engine::OnDestroy()
	{
		return true;
	}
}