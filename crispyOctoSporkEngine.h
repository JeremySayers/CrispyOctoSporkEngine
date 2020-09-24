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
	/// Base class for objects that should be updated and rendered.
	/// </summary>
	class Entity
	{
	public:
		/// <summary>
		/// Default constructor.
		/// </summary>
		Entity();

		/// <summary>
		/// Constructs a new instance of <see cref="Entity"/>.
		/// </summary>
		/// <param name="x">The x location of the entity in the engine.</param>
		/// <param name="y">The y location of the entity in the engine.</param>
		Entity(float x, float y);

		/// <summary>
		/// Default deconstructor.
		/// </summary>
		~Entity();

		/// <summary>
		/// Called once a frame to be overridden by derived classes.
		/// </summary>
		/// <param name="deltaTime">The delta time since the last frame.</param>
		/// <returns>Returns a boolean.</returns>
		virtual bool OnUpdate(float deltaTime);

		/// <summary>
		/// Called once a frame to be overridden by derived classes.
		/// </summary>
		/// <param name="deltaTime">The delta time since the last frame.</param>
		/// <returns>Returns a boolean.</returns>
		virtual bool OnRender(float deltaTime);

	protected:
		float x;
		float y;
	};

	/// <summary>
	/// A few SDL_Colors to use.
	/// </summary>
	SDL_Color COLOR_RED = { 255, 0, 0, 255 }, COLOR_GREEN = { 0, 255, 0, 255 }, COLOR_BLUE = { 0, 0, 255, 255 };

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

		/// <summary>
		/// Adds an <see cref="Entity"/> to the engines vector of entites. Will be cleaned up by the engine.
		/// </summary>
		/// <param name="entity">The entity pointer to add to the vector.</param>
		void AddEntity(Entity* entity);

	protected:
		SDL_Window* window;
		SDL_Renderer* renderer;
		int screenWidth;
		int screenHeight;
		bool isVsyncEnabled;
		bool isFullscreenEnabled;
		std::string name;
		bool isEngineRunning;
		std::vector <Entity*> entities;

		/// <summary>
		/// The main loop. To support emscripten the current <see cref="Engine"/> instance is passed in.
		/// </summary>
		/// <param name="arg">A pointer to the current <see cref="Engine"/> instance.</param>
		static void Update(void* arg);
	};

	/// <summary>
	/// An object for holding texture information and to perform basic renders of it.
	/// </summary>
	class Texture
	{
	public:
		/// <summary>
		/// Default constructor.
		/// </summary>
		Texture();

		/// <summary>
		/// Creates a new instance of <see cref="Texture"/>.
		/// </summary>
		Texture(SDL_Renderer* renderer);

		/// <summary>
		/// Default deconstructor.
		/// </summary>
		~Texture();

		/// <summary>
		/// Populates the <see cref="SDL_Texture"/> from a file.
		/// </summary>
		/// <param name="filepath">The file path to load an image from for the texture.</param>
		/// <returns>Returns a boolean indicating success.</returns>
		bool LoadTextureFromFile(const char* filepath);

		/// <summary>
		/// Called to cleanup and free the texture.
		/// </summary>
		void Free();

		/// <summary>
		/// Renders the texture to the current <see cref="SDL_Renderer"/>.
		/// </summary>
		/// <param name="x">The x location to draw to.</param>
		/// <param name="y">The y location to draw to.</param>
		/// <param name="clip">The rectangle to pull texture information from. Useful for sprite sheets.</param>
		/// <param name="angle">The angle at which to rotate the texture.</param>
		/// <param name="center">The center to rotate about.</param>
		/// <param name="flip">Determines if the texture will be flipped.</param>
		void Render(double x, double y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_FPoint* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

		/// <summary>
		/// Gets the current renderer associated with this texture.
		/// </summary>
		/// <returns>Returns a pointer to a <see cref="SDL_Renderer"/>.</returns>
		SDL_Renderer* GetRenderer();

	private:
		SDL_Texture* texture;
		SDL_Renderer* renderer;
		int width;
		int height;
	};

	/// <summary>
	/// Sprite class to hold an entity and a texture.
	/// </summary>
	class Sprite : public Entity
	{
	public:
		/// <summary>
		/// Default constructor.
		/// </summary>
		Sprite();

		/// <summary>
		/// Creates a new instance of <see cref="Sprite"/>.
		/// </summary>
		/// <param name="x">The x location of the sprite in the engine.</param>
		/// <param name="y">The y location of the sprite in the engine.</param>
		/// <param name="width">The width of the sprite.</param>
		/// <param name="height">The height of the sprite.</param>
		/// <param name="texture">A pointer to the texture to use for the sprite.</param>
		/// <param name="renderer">A pointer to the renderer to render the sprite to.</param>
		Sprite(float x, float y, float width, float height, Texture* texture, SDL_Renderer* renderer);

		/// <summary>
		/// Called once per frame. Renders the sprite to its current location.
		/// </summary>
		/// <param name="deltaTime">The delta time since the last frame.</param>
		/// <returns>Returns a boolean indicating success.</returns>
		virtual bool OnRender(float deltaTime);

	protected:
		float width;
		float height;
		Texture* texture;
		SDL_Renderer* renderer;

	};

	/// <summary>
	/// Class to hold an entity represented by a rectangle.
	/// </summary>
	class Rectangle : public Entity
	{
	public:
		/// <summary>
		/// Default constructor.
		/// </summary>
		Rectangle();

		/// <summary>
		/// Creates a new instance of <see cref="Rectangle"/>.
		/// </summary>
		/// <param name="x">The x location of the rectangle in the engine.</param>
		/// <param name="y">The y location of the rectangle in the engine.</param>
		/// <param name="width">The width of the rectangle.</param>
		/// <param name="height">The height of the rectangle.</param>
		/// <param name="color">The color to draw the rectangle with.</param>
		/// <param name="renderer">A pointer to the renderer to render the rectangle to.</param>
		Rectangle(float x, float y, float width, float height, SDL_Color color, SDL_Renderer* renderer);

		/// <summary>
		/// Called once per frame. Renders the rectangle to its current location.
		/// </summary>
		/// <param name="deltaTime">The delta time since the last frame.</param>
		/// <returns>Returns a boolean indicating success.</returns>
		virtual bool OnRender(float deltaTime);

	protected:
		float width;
		float height;
		SDL_Color color;
		SDL_Renderer* renderer;
	};

	/// <summary>
	/// Class to hold an entity represented by a circle.
	/// </summary>
	class Circle : public Entity
	{
	public:
		/// <summary>
		/// Default constructor.
		/// </summary>
		Circle();

		/// <summary>
		/// Creates a new instance of <see cref="Circle"/>.
		/// </summary>
		/// <param name="x">The x location of the center of the circle.</param>
		/// <param name="y">The y location of the center of the circle.</param>
		/// <param name="radius">The radius of the circle</param>
		/// <param name="color">The color to draw the circle with.</param>
		/// <param name="renderer">A pointer to the renderer to render the circle to.</param>
		Circle(float x, float y, float radius, SDL_Color color, SDL_Renderer* renderer);

		/// <summary>
		/// Called once per frame. Renders the circle to its current location.
		/// </summary>
		/// <param name="deltaTime">The delta time since the last frame.</param>
		/// <returns>Returns a boolean indicating success.</returns>
		virtual bool OnRender(float deltaTime);

	protected:
		float radius;
		SDL_Color color;
		SDL_Renderer* renderer;
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

		OnDestroy();

		return true;
	}


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
		for (auto& entity : entities)
		{
			entity->OnRender(deltaTime);
		}

		return true;
	}

	bool Engine::OnDestroy()
	{
		for (auto p : entities)
		{
			delete p;
		}

		entities.clear();
		return true;
	}

	void Engine::AddEntity(Entity* entity)
	{
		entities.push_back(entity);
	}

	Entity::Entity()
	{
		x = 0;
		y = 0;
	}

	Entity::Entity(float x, float y)
	{
		this->x = x;
		this->y = y;
	}

	Sprite::Sprite()
	{
		this->width = 0;
		this->height = 0;
		this->texture = NULL;
		this->renderer = NULL;
	}

	Sprite::Sprite(float x, float y, float width, float height, Texture* texture, SDL_Renderer* renderer) : Entity(x, y)
	{
		this->width = width;
		this->height = height;
		this->texture = texture;
		this->renderer = renderer;
	}

	bool Sprite::OnRender(float deltaTime)
	{
		texture->Render(x, y);

		return true;
	}

	Rectangle::Rectangle()
	{
		this->width = 0;
		this->height = 0;
		this->color = SDL_Color{255, 255, 255, 255};
		this->renderer = NULL;
	}

	Rectangle::Rectangle(float x, float y, float width, float height, SDL_Color color, SDL_Renderer* renderer) : Entity(x, y)
	{
		this->width = width;
		this->height = height;
		this->color = color;
		this->renderer = renderer;
	}

	bool Rectangle::OnRender(float deltaTime)
	{
		SDL_SetRenderDrawColor(renderer, color.r, color.b, color.g, color.a);
		SDL_FRect rect = { x, y, width, height };
		SDL_RenderFillRectF(renderer, &rect);
		return false;
	}

	Circle::Circle()
	{
		this->radius = 0;
		this->color = SDL_Color{ 255, 255, 255, 255 };
		this->renderer = NULL;
	}

	Circle::Circle(float x, float y, float radius, SDL_Color color, SDL_Renderer* renderer) : Entity(x, y)
	{
		this->radius = radius;
		this->color = color;
		this->renderer = renderer;
	}

	bool Circle::OnRender(float deltaTime)
	{
		SDL_SetRenderDrawColor(renderer, color.r, color.b, color.g, color.a);
		for (int w = 0; w < radius * 2; w++)
		{
			for (int h = 0; h < radius * 2; h++)
			{
				int dx = radius - w; // horizontal offset
				int dy = radius - h; // vertical offset
				if ((dx * dx + dy * dy) <= (radius * radius))
				{
					SDL_RenderDrawPoint(renderer, x + dx, y + dy);
				}
			}
		}
		return false;
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
		 return true;
	 }

	Texture::Texture()
	{
		this->texture = NULL;
		this->renderer = NULL;
		this->width = 0;
		this->height = 0;
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

	bool Texture::LoadTextureFromFile(const char* filepath)
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