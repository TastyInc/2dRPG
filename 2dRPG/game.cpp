#include "Game.hpp"
#include "Map.hpp"
#include "Components.hpp"
#include "Collision.hpp"
#include <sstream>

Map* map;
Manager manager;
MenuHandler menuHandler;

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;

AssetManager* Game::assets = new AssetManager(&manager);
SceneManager* Game::scenes = new SceneManager(&menuHandler);
CameraHandler* Game::camera = new CameraHandler(0, 0, Game::WINDOW_HEIGHT, Game::WINDOW_WIDTH );
SavefileHandler* Game::savegame = new SavefileHandler();
HUDManager* Game::hud = new HUDManager();

bool Game::isRunning = false;

auto& player(manager.addEntity());

Game::Game() {}

Game::~Game() {}

void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen) {
	
	int flags = 0;
	if (fullscreen) {
		flags = SDL_WINDOW_FULLSCREEN;
	}
	
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {

		std::cout << "Subsystems Initialized!" << std::endl;

		window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
		if (window) {
			std::cout << "Window created!" << std::endl;
			
			renderer = SDL_CreateRenderer(window, -1, 0);
			if (renderer) {
				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
				SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
				std::cout << "Renderer created!" << std::endl;

				isRunning = true;
			}
		}
	} else {
		isRunning = false;
	}

	if (TTF_Init() == -1) {
		std::cout << "ERROR: SDL_TTF" << std::endl;
	}

	//angers positioniere de no
	if (savegame->loadGame() != 1) {
		std::cout << "COULD NOT LOAD DATA FROM SAVE FILE!" << std::endl;
	}

	//----------------
	assets->AddTexture("projectile", "resources/sprites/proj.png");

	assets->AddTexture("enemy", "resources/sprites/enemy1.png");

	assets->AddFont("arial", "resources/font/arial.ttf", 30);
	//----------------


	map = new Map();

	map->LoadMap(savegame->mapTheme, savegame->mapScreen);


	assets->AddTexture("player", "resources/sprites/new_player_idle.png");
	player.addComponent<TransformComponent>(savegame->playerPos.x, savegame->playerPos.y, 64, 64, 2);
	player.addComponent<SpriteComponent>("player", true);
	player.addComponent<KeyboardController>();
	player.addComponent<ColliderComponent>("player", 40, 80, 48, 48, true);
	player.addComponent<CharacterComponent>(100, 100);
	player.addGroup(groupPlayers);

	mTimer = Timer::Instance();

	//----------------
	//texts.addComponent<UILable>(10, 10, "test string", "arial", white);
	//assets->createText("test SUPER", {50, 50}, "arial", white);

	//ADD PROJECTILES HERE
	assets->createProjectile(Vector2D(1000, 1000),Vector2D(2, 0), 300, 2, "projectile");
	assets->createEnemy(1, Vector2D(1400, 1200));

	//----------------


	//LOAD into assets HUD
	//HUd söt sech ar kamera festhebe, nid ar map
	if (hud->loadHUD()) {
		for (int i = 0; i < hud->elemCount - 1; i++) {
			assets->AddTexture(hud->name[i], hud->sprite[i]);
			assets->createHud(hud->name[i], Vector2D(hud->posx[i], hud->posy[i]), hud->sizex[i], hud->sizey[i]);
		}
		delete hud;
	}

	camera->camera.x = 100; //<-??
}

//list of all entities for updating
auto& tiles(manager.getGroup(Game::groupMap));
auto& players(manager.getGroup(Game::groupPlayers));
auto& colliders(manager.getGroup(Game::groupColliders));
auto& projectiles(manager.getGroup(Game::groupProjectiles));
auto& texts(manager.getGroup(Game::groupTexts));
auto& enemies(manager.getGroup(Game::groupEnemies));
auto& hudElements(manager.getGroup(Game::groupHUD));

void Game::handleEvents() {

	while (SDL_PollEvent(&event)) {
		if (scenes->getCurrentScene() == 0) {
			player.getComponent<KeyboardController>().keyInput();
		} else {
			player.getComponent<KeyboardController>().keyInputMenu();
		}
	}
}

void Game::update() { 
	
	mTimer->Update();

	player.getComponent<KeyboardController>().setVelocity();

	SDL_Rect playerCol = player.getComponent<ColliderComponent>().collider;
	Vector2D playerPos = player.getComponent<TransformComponent>().position;

	manager.refresh();
	manager.update();

	player.getComponent<CharacterComponent>().updateStamina(player.getComponent<KeyboardController>().isSprinting());

	//check player collider against map collider and resets player position if true
	for (auto& c : colliders) {
		SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;

		switch (Collision::AABBxy(cCol, playerCol)){
			case 0:
				break;
			case 1:
				player.getComponent<TransformComponent>().position.x = cCol.x + cCol.w - player.getComponent<ColliderComponent>().xOffset + 1.0f;
				break;
			case 3:
				player.getComponent<TransformComponent>().position.x = cCol.x - playerCol.w - player.getComponent<ColliderComponent>().xOffset - 1.0f;
				break;
			case 2:
				player.getComponent<TransformComponent>().position.y = cCol.y + cCol.h - player.getComponent<ColliderComponent>().yOffset + 1.0f;
				break;
			case 4:
				player.getComponent<TransformComponent>().position.y = cCol.y - playerCol.h - player.getComponent<ColliderComponent>().yOffset - 1.0f;
				break;

			default:
				break;
		}
	}

	//collision projectiles
	for (auto& p : projectiles) {
		if (Collision::AABB(player.getComponent<ColliderComponent>().collider, p->getComponent<ColliderComponent>().collider)) {
			//health etc. here
			p->destroy();
		}
	}

	if (playerPos.y + 128 <= 0 || playerPos.x + 128 <= 0 || playerPos.x >= map->getMapSize().x || playerPos.y >= map->getMapSize().y ) {
		Game::newScreen(playerPos);
	}

	camera->updateCameraPos(static_cast<int>(playerPos.x - WINDOW_WIDTH / 2 + player.getComponent<TransformComponent>().width / 2), static_cast<int>(playerPos.y - WINDOW_HEIGHT / 2 + player.getComponent<TransformComponent>().height / 2));
}

void Game::render() {
	SDL_RenderClear(renderer);
	
	for (auto& t : tiles) { //for each T in tiles
		t->draw();
	}

	for (auto& c : colliders) { //for each T in tiles
		c->draw();
	}

	for (auto& p : players) { //for each T in tiles
		p->draw();
	}

	for (auto& e : enemies) { //for each T in tiles
		e->draw();
	}

	for (auto& p : projectiles) {
		p->draw();
	}

	for (auto& h : hudElements) {
		h->draw();
	}

	for (auto& t : texts) {
		t->draw();
	}


	SDL_RenderPresent(renderer);
}

void Game::renderMenu() {
	SDL_RenderClear(renderer);

	

	/*
	TTF_Font* Sans = TTF_OpenFont("resources/font/arial.ttf", 24);

	SDL_Surface* surf = TTF_RenderText_Blended(Sans, "THIS IS A TEST STRING", {255, 255, 255, 255});
	SDL_Texture* labelTexture; labelTexture = SDL_CreateTextureFromSurface(Game::renderer, surf);
	SDL_FreeSurface(surf);
	SDL_Rect rekt = { 100, 100, 500, 100 };

	SDL_RenderCopy(renderer, labelTexture, NULL, &rekt);
	//----
	SDL_QueryTexture(labelTexture, nullptr, nullptr, 0, 0);
	*/


	SDL_Texture* background = IMG_LoadTexture(renderer, "resources/sprites/menu_background.png");

	SDL_RenderCopy(renderer, background, NULL, NULL);

	SDL_SetRenderDrawColor(renderer, 150, 150, 150, 150);
	menuButton.y = scenes->menus->activeButton * 60 + 440;
	SDL_RenderFillRect(renderer, &menuButton);

	for (auto& t : texts) {
		t->draw();
	}

	scenes->getNewScene();
	
	SDL_RenderPresent(renderer);

}

void Game::newScene() {
	
	for (auto& t : texts) {
		t->destroy();
	}

	int scene = scenes->getNewScene();

	if (scene == 9) {
		//hie filech no frage 
		Game::isRunning = false;
	} else if(scene == 8){
		//speichert spiel und geht ins hauptmenu
		savegame->saveGame(player.getComponent<TransformComponent>().position, map->getMapThemeID(), map->getMapScreenID());
		scenes->setScene(1);
	} else {
		scenes->setScene(scene);
		if (scene != 0) {
			for (int i = 0; i < scenes->menus->eleCount; i++) {
				assets->createText(scenes->menus->menuText[i], { 108, float(i) * 60 + 503 }, "arial", white);
			}
			menuButton = { 100, 500, 500, 40 };
			scenes->menus->activeButton = 1;
		} else {
			camera->updateCameraSize(map->getMapSize(), WINDOW_WIDTH, WINDOW_HEIGHT);
		}
	}
	
	manager.refresh();
}

void Game::newScreen(Vector2D pp) {
	Vector2D playerPos = pp;

	for (auto& t : tiles) { //for each T in tiles
		t->delGroup(Game::groupMap);
	}

	for (auto& c : colliders) { //for each T in tiles
		c->delGroup(Game::groupColliders);
	}

	//North
	if (playerPos.y + 128 <= 0) {
		direction = "n";
	}
	//WEst
	else if (playerPos.x + 128 <= 0) {
		direction = "w";
	}
	//East
	else if (playerPos.x >= map->getMapSize().x) {
		direction = "e";
	}
	//South
	else if (playerPos.y >= map->getMapSize().y) {
		direction = "s";
	}
	//Action (Boat, Boss, Event, Hole etc)
	else {
		direction = "";
	}
	if (direction != "") {
		map->LoadMap(direction);
	} else {
		//eventNumber oder so
		map->LoadMap(1);
	}

	player.getComponent<TransformComponent>().position = map->spawnPoint;
	camera->updateCameraSize(map->getMapSize(), WINDOW_WIDTH, WINDOW_HEIGHT);



}

void Game::clean() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	std::cout << "Game Cleaned" << std::endl;
}
