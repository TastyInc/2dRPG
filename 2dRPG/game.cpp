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

bool Game::isRunning = false;

auto& player(manager.addEntity());
//auto& texts(manager.addEntity());

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

	assets->AddTexture("player", "resources/sprites/new_player_idle.png");
	assets->AddTexture("projectile", "resources/sprites/proj.png");

	assets->AddFont("arial", "resources/font/arial.ttf", 30);

	map = new Map();

	map->LoadMap(1, 1);

	player.addComponent<TransformComponent>(800, 800, 64, 64, 2);
	player.addComponent<SpriteComponent>("player", true);
	player.addComponent<KeyboardController>();
	player.addComponent<ColliderComponent>("player", 40, 80, 48, 48, true);
	player.addGroup(groupPlayers);

	//texts.addComponent<UILable>(10, 10, "test string", "arial", white);
	assets->createText("test SUPER", {50, 50}, "arial", white);

	//ADD PROJECTILES HERE
	assets->createProjectile(Vector2D(1000, 1000),Vector2D(2, 0), 300, 2, "projectile");

	camera->camera.x = 100;

}

//list of all entities for updating
auto& tiles(manager.getGroup(Game::groupMap));
auto& players(manager.getGroup(Game::groupPlayers));
auto& colliders(manager.getGroup(Game::groupColliders));
auto& projectiles(manager.getGroup(Game::groupProjectiles));
auto& texts(manager.getGroup(Game::groupTexts));

void Game::handleEvents() {

	SDL_PollEvent(&event);

	if (scenes->getCurrentScene() == 0) {
		player.getComponent<KeyboardController>().keyInput();
	} else {
		player.getComponent<KeyboardController>().keyInputMenu();
	}


}

void Game::update() { 
	
	player.getComponent<KeyboardController>().setVelocity();

	SDL_Rect playerCol = player.getComponent<ColliderComponent>().collider;
	Vector2D playerPos = player.getComponent<TransformComponent>().position;

	//label.getComponent<UILable>().SetLableText("Test", "arial");

	manager.refresh();
	manager.update();

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

	for (auto& p : projectiles) {
		p->draw();
	}

	for (auto& t : texts) {
		t->draw();
	}

	SDL_RenderPresent(renderer);
}

void Game::renderMenu() {
	SDL_RenderClear(renderer);

	
	SDL_Texture* background = IMG_LoadTexture(renderer, "resources/sprites/firewatch.png");

	SDL_RenderCopy(renderer, background, NULL, NULL);


	//verschiebe das text nid immer wiede rneu erstellt wird. oder create text abendere. ganzes renderzügs überarbeite verdammt
	for (int i = 0; i < scenes->menus->eleCount; i++){
		SDL_SetRenderDrawColor(renderer, 150, 150, 150, scenes->menus->buttonColor(i+1));
		SDL_RenderFillRect(renderer, &scenes->menus->menuRect[i]);
		assets->createText(scenes->menus->menuText[i], { 108, float(i) * 60 + 503 }, "arial", white);
	}

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

	if (scenes->getNewScene() != 0) {
		scenes->setScene(scenes->getNewScene());
	} else {
		scenes->setScene(scenes->getNewScene());
		camera->updateCameraSize(map->getMapSize(), WINDOW_WIDTH, WINDOW_HEIGHT);
	}
}

void Game::clean() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	std::cout << "Game Cleaned" << std::endl;
}
