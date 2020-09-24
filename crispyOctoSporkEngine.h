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

	protected:
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
		static void Update(void* arg);
	};

	class Texture
	{
	public:
		Texture();
		Texture(SDL_Renderer* renderer);
		~Texture();
		bool LoadTextureFromFile(SDL_Renderer* renderer, const char* filepath);
		void Free();
		void Render(double x, double y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_FPoint* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
		SDL_Renderer* GetRenderer();
	private:
		SDL_Texture* texture;
		SDL_Renderer* renderer;
		int width;
		int height;
	};

	class Entity
	{
	public:
		Entity();
		Entity(float width, float height, float x, float y, Texture* texture = NULL);
		~Entity();
		virtual bool OnUpdate(float deltaTime);
		virtual bool OnRender(float deltaTime);
		SDL_Rect GetRect();
	private:
		float width;
		float height;
		float x;
		float y;
		Texture* texture;
	};

	Engine::Engine()
	{
		SDL_Init(SDL_INIT_VIDEO);

		window = NULL;
		renderer = NULL;
		screenWidth = 0;
		screenHeight = 0;
		isVsyncEnabled = true;
		isFullscreenEnabled = false;
		name = "";
		isEngineRunning = false;
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

		return true;
	}

	/// <summary>
	/// Called to start the main game loop.
	/// </summary>
	/// <returns>Returns a boolean.</returns>
	bool Engine::Start()
	{
		isEngineRunning = true;

		this->OnCreate();

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

	/// <summary>
	/// The main loop. To support emscripten the current <see cref="Engine"/> instance is passed in.
	/// </summary>
	/// <param name="arg">A pointer to the current <see cref="Engine"/> instance.</param>
	void Engine::Update(void* arg)
	{
		Engine* engine = (Engine*)arg;

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

		engine->OnUpdate(1.0);

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

	Entity::Entity()
	{}

	Entity::Entity(float width, float height, float x, float y, Texture* texture)
	{
		this->width = width;
		this->height = height;
		this->x = x;
		this->y = y;
		this->texture = texture;
	}

	Entity::~Entity()
	{
	}

	 bool Entity::OnUpdate(float deltaTime)
	{
		return true;
	}

	bool Entity::OnRender(float deltaTime)
	{
		if (texture != NULL)
		{
			texture->Render(x, y);
		}

		return true;
	}

	SDL_Rect Entity::GetRect()
	{
		SDL_Rect rect = { x, y, width, height };
		return rect;
	}

	Texture::Texture()
	{
		this->texture = NULL;
		this->renderer = NULL;
	}

	Texture::Texture(SDL_Renderer* renderer)
	{
		this->texture = NULL;
		this->width = 0;
		this->height = 0;
		this->renderer = renderer;
	}

	Texture::~Texture()
	{
		Free();
	}

	bool Texture::LoadTextureFromFile(SDL_Renderer* renderer, const char* filepath)
	{
		Free();

		texture = IMG_LoadTexture(renderer, filepath);

		if (texture == NULL)
		{
			std::cout << "Could not load the texture from: " << filepath << " Error:" << SDL_GetError() << std::endl;
		}

		SDL_QueryTexture(texture, NULL, NULL, &width, &height);

		return true;
	}

	void Texture::Free()
	{
		if (texture != NULL)
		{
			SDL_DestroyTexture(texture);
			texture = NULL;
			width = 0;
			height = 0;
		}
	}

	void Texture::Render(double x, double y, SDL_Rect* clip, double angle, SDL_FPoint* center, SDL_RendererFlip flip)
	{
		SDL_FRect renderQuad = { x, y, width, height };

		if (clip != NULL)
		{
			renderQuad.w = clip->w;
			renderQuad.h = clip->h;
		}

		SDL_RenderCopyExF(renderer, texture, clip, &renderQuad, angle, center, flip);
	}
	SDL_Renderer* Texture::GetRenderer()
	{
		return renderer;
	}
}