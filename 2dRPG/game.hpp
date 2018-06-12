#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include <iostream>
#include <vector>
#include "AssetManager.hpp"
#undef main

class AssetManager;
class ColliderComponent;

class Game {
public:
	Game();
	~Game();
	
	void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);

	void handleEvents();
	void update();
	bool running() { return isRunning; };
	void render();
	void clean();
	
	static AssetManager* assets;
	static SDL_Renderer *renderer;
	static SDL_Event event;
	static bool isRunning;
	static SDL_Rect camera;

	const static int WINDOW_WIDTH = 1280;
	const static int WINDOW_HEIGHT = 864;

	enum groupLabels : std::size_t {
		groupMap,
		groupPlayers,
		groupColliders
	};


private:
	SDL_Window *window;
};
