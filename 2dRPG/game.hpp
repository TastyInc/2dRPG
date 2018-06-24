#pragma once
#include "SDL.h"
#include "SDL_image.h"

#include <iostream>
#include <vector>

#include "Vector2D.hpp"
#include "AssetManager.hpp"
#include "SceneManager.hpp"
#include "TextureManager.hpp"

#include "CameraHandler.hpp"
#include "SavefileHandler.hpp"

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
	void renderMenu();
	void newScene();
	
	static AssetManager* assets;
	static SceneManager* scenes;
	static CameraHandler* camera;
	static SavefileHandler* savegame;

	static SDL_Renderer *renderer;
	static SDL_Event event;

	const static int WINDOW_WIDTH = 1280;
	const static int WINDOW_HEIGHT = 864;

	static bool isRunning;

	//Scene as in which menu, map, event etc..
	int scene = 0;

	enum groupLabels : std::size_t {
		groupMap,
		groupPlayers,
		groupColliders,
		groupProjectiles,
		groupEnemies,
		groupTexts,
	};


private:
	SDL_Window *window;
	const char* playerSprite;
	SDL_Color white = { 255, 255, 255, 255 };
	SDL_Rect menuButton;

};
