# CrispyOctoSporkEngine

## What is it?
Simple game engine written in c++ that abstracts some SDL functionality, along with adding support for common game engine needs.

## Basic Setup
To use with VS 2019, you'll need to download the `crispyOctoSporkEngine.h` header file and add it to your project.
Then you'll need `SDL2` and `SDL2_image` added as includes and linked against.

## Emscripten support
To compile with emscripten for a web build, you'll need the header file, along with the optional blank skeleton html.
Then using
```
emcc sampleGame.cpp -O2 -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='["png"]' -o index.html --shell-file emcc_skeleton.html
```
or replace `sampleGame.cpp` with `(get-item .\*.cpp).FullName` to get all the cpp files in the current dir (in powershell)
