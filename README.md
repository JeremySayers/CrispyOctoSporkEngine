# CrispyOctoSporkEngine
Simple game engine written in c++ that abstracts some SDL functionality, along with adding support for common game engine needs.
Below shows a sceenshot of the sample game that renders entites as textures, circles, and rectangles. Along with the basic particle system.

![Screenshot](https://github.com/JeremySayers/CrispyOctoSporkEngine/blob/master/image.png?raw=true)

## Basic Setup
To use with VS 2019, you'll need to download the `crispyOctoSporkEngine.h` header file and add it to your project.
Then you'll need `SDL2` and `SDL2_image` added as includes and linked against.

There is a `sampleGame.cpp` above that will get you started. But at a minimum you'll need to create a class deriving from `Engine` and override `OnCreate()` and `OnUpdate` like below:
```c++
class SampleGame : public Engine
{
public:
	bool OnCreate() override
	{
		return true;
	}
  
	bool OnUpdate(float deltaTime) override
	{
		return true;
	}
};
```

Then your main method would look like:
```c++
int main(int argc, char* argv[])
{
	SampleGame game;

	if (game.Create("SampleGame", 1280, 720, true, false))
	{
		game.Start();
	}

	return 0;
}
```

Currently a very lightweight Entity system is in place where you can create a `Sprite`, `Rectangle`, or `Circle`. These can be added to the engine with a call like:
```c++
AddEntity(new Rectangle(0, 0, 32, 32, COLOR_GREEN, renderer));
```

The above mentioned Entity types all have a default `OnRender` that will be called in the engines base implementation `OnUpdate()` so you'll need to add a call to your overriden `OnUpdate()` in order to get the free renderering like:
```c++
bool OnUpdate(float deltaTime) override
{
	Engine::OnUpdate(deltaTime);
	return true;
}
```

## Emscripten support
To compile with emscripten for a web build, you'll need the header file, along with the optional blank skeleton html.
Then using
```
emcc sampleGame.cpp -O2 -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='["png"]' --preload-file assets -o index.html --shell-file emcc_skeleton.html
```
or replace `sampleGame.cpp` with `(get-item .\*.cpp).FullName` to get all the cpp files in the current dir (in powershell)
