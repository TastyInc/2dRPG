#ifndef Game_hpp
#define Game_hpp

#include "SDL.h"
#include "SDL_image.h"
#include <iostream>
#include <vector>
#undef main

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
	
	const static int WINDOW_WIDTH = 1600;
	const static int WINDOW_HEIGHT = 1120;

	static SDL_Renderer *renderer;
	static SDL_Event event;
	static bool isRunning;
	static SDL_Rect camera;

	enum groupLabels : std::size_t {
		groupMap,
		groupPlayers,
		groupColliders
	};


private:
	SDL_Window *window;
};

#endif /* Game_hpp*/