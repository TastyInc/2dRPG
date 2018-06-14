#include "Game.hpp"
#include "TextureManager.hpp"
#include "Map.hpp"
#include "Components.hpp"
#include "Vector2D.hpp"
#include "Collision.hpp"
#include "AssetManager.hpp"
#include <sstream>

Map* map;
Manager manager;

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;

SDL_Rect Game::camera = { 0, 0, Game::WINDOW_HEIGHT, Game::WINDOW_WIDTH };

AssetManager* Game::assets = new AssetManager(&manager);
SceneManager* Game::scene = new SceneManager();

bool Game::isRunning = false;

auto& player(manager.addEntity());
auto& label(manager.addEntity());


Game::Game() {

}

Game::~Game() {

}

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
				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 5);
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

	assets->AddTexture("player", "assets/new_player_idle.png");
	assets->AddTexture("projectile", "assets/proj.png");

	assets->AddFont("arial", "assets/arial.ttf", 30);

	map = new Map();

	map->LoadMap(1, 1);

	player.addComponent<TransformComponent>(800, 800, 64, 64, 2);
	player.addComponent<SpriteComponent>("player", true);
	player.addComponent<KeyboardController>();
	player.addComponent<ColliderComponent>("player", 40, 80, 48, 48, true);
	player.addGroup(groupPlayers);
	playerVel.Zero();

	SDL_Color white = { 255, 255, 255, 255 };
	label.addComponent<UILable>(10, 10, "test string", "arial", white);

	//ADD PROJECTILES HERE
	assets->createProjectile(Vector2D(1000, 1000),Vector2D(2, 0), 300, 2, "projectile");

	camera.h = 35 * 32 * 3 - WINDOW_HEIGHT;
	camera.w = 60 * 32 * 3 - WINDOW_WIDTH;
}

//list of all entities for updating
auto& tiles(manager.getGroup(Game::groupMap));
auto& players(manager.getGroup(Game::groupPlayers));
auto& colliders(manager.getGroup(Game::groupColliders));
auto& projectiles(manager.getGroup(Game::groupProjectiles));

void Game::handleEvents() {

	SDL_PollEvent(&event);

	switch (event.type) {
	case SDL_QUIT:
		isRunning = false;
		break;
	case SDL_KEYDOWN:
		switch (event.key.keysym.sym) {
			case SDLK_w:
				playerVel.y = -1;
				break;
			case SDLK_s:
				playerVel.y = 1;
				break;
			case SDLK_a:
				playerVel.x = -1;
				break;
			case SDLK_d:
				playerVel.x = 1;
				break;
			case SDLK_o:
				Game::scene->setScene(2);
			default:
				break;
		}
	}

	switch (event.type) {
	case SDL_KEYUP:			
		switch (Game::event.key.keysym.sym) {
			case SDLK_w:
			case SDLK_s:
				playerVel.y = 0;
				break;
			case SDLK_a:
			case SDLK_d:
				playerVel.x = 0;
				break;
			case SDLK_ESCAPE:
				Game::isRunning = false;
				break;
			default:
				break;
			}	
	default:
		break;
	}
}

void Game::update() { 
	
	player.getComponent<KeyboardController>().setVelocity(playerVel.x, playerVel.y);

	SDL_Rect playerCol = player.getComponent<ColliderComponent>().collider;
	Vector2D playerPos = player.getComponent<TransformComponent>().position;
	
	//std::stringstream strstr;

	//strstr << "Player Position: " << playerPos;

	label.getComponent<UILable>().SetLableText("Test", "arial");

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

	camera.x = static_cast<int>(player.getComponent<TransformComponent>().position.x - WINDOW_WIDTH / 2);
	camera.y = static_cast<int>(player.getComponent<TransformComponent>().position.y - WINDOW_HEIGHT / 2);

	if (camera.x < 0){
		camera.x = 0;
	}
	if (camera.y < 0){
		camera.y = 0;
	}
	if (camera.x > camera.w){
		camera.x = camera.w;
	}
	if (camera.y > camera.h){
		camera.y = camera.h;
	}
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

	label.draw();

	SDL_RenderPresent(renderer);
}

void Game::renderMenu() {

}

void Game::clean() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	std::cout << "Game Cleaned" << std::endl;
}
