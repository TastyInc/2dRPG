#pragma once
#include "SDL.h"
#include "SDL_image.h"

#include <iostream>
#include <vector>
#include "Timer.hpp"

#include "Vector2D.hpp"
#include "AssetManager.hpp"
#include "SceneManager.hpp"
#include "TextureManager.hpp"
#include "HUDManager.hpp"

#include "CameraHandler.hpp"
#include "SavefileHandler.hpp"
#include "SpellHandler.hpp"

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
	void newScreen(Vector2D pp);
	
	static AssetManager* assets;
	static SceneManager* scenes;
	static CameraHandler* camera;
	static SavefileHandler* savegame;
	static SpellHandler* spellHandler;

	static SDL_Renderer *renderer;
	static SDL_Event event;

	const static int WINDOW_WIDTH = 1280;
	const static int WINDOW_HEIGHT = 864;

	static bool isRunning;

	//Scene as in which menu, map, event etc..
	int scene = 0;
	int scaleMultiplier;

	//multiplier for enemies, character, sprites, everything!
	float drawingScale;

	enum groupLabels : std::size_t {
		groupMap,
		groupPlayers,
		groupColliders,
		groupProjectiles,
		groupEnemies,
		groupTexts,
		groupHUD,
		groupSpells
	};

	enum subgroupLabels : std::size_t {
		subHudMana,
		subHudHealth,
		subHudStamina
	};


private:
	SDL_Window *window;
	const char* playerSprite;
	SDL_Color white = { 255, 255, 255, 255 };
	SDL_Rect menuButton;

	Timer* mTimer;

};
