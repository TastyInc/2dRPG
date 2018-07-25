#include "Game.hpp"
#include "Map.hpp"
#include "Components.hpp"
#include "Collision.hpp"
#include <sstream>
#include "KeyboardController.hpp"

Map* map;
Manager manager;
MenuHandler menuHandler;

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;

AssetManager* Game::assets = new AssetManager(&manager);
SceneManager* Game::scenes = new SceneManager(&menuHandler);
CameraHandler* Game::camera = new CameraHandler(0, 0, Game::WINDOW_HEIGHT, Game::WINDOW_WIDTH );
SavefileHandler* Game::savegame = new SavefileHandler();
SpellHandler* Game::spellHandler = new SpellHandler();
EnemyHandler* Game::enemyHandler = new EnemyHandler();


bool Game::isRunning = false;

auto& player(manager.addEntity());

KeyboardController keyboardController;

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

	mTimer = Timer::Instance();

	//----------------
	//texts.addComponent<UILable>(10, 10, "test string", "arial", white);
	//assets->createText("test SUPER", {50, 50}, "arial", white);

	//ADD PROJECTILES HERE
	//assets->createProjectile(Vector2D(1000, 1000),Vector2D(2, 1), 300, 2, "projectile");
	enemyHandler->createEnemy();

	//----------------
	assets->AddTexture("player", "resources/sprites/new_player_idle.png");
	player.addComponent<TransformComponent>(/*savegame->playerPos.x*/ 200, /*savegame->playerPos.y*/ 100, 64, 64, 1);
	player.addComponent<SpriteComponent>("player", true);
	//player.addComponent<KeyboardController>();
	//hie no apasse

	player.addComponent<ColliderComponent>("player", 20, 40, 24, 24, true);
	player.addComponent<PlayerComponent>(100, 100, 100);
	player.addGroup(groupPlayers);

	//LOAD into assets HUD
	//HUd söt sech ar kamera festhebe, nid ar map
	HUDManager Hud;
	if (Hud.loadHUD()) {
		while (Hud.getHudElement()){
			assets->AddTexture(Hud.name, Hud.sprite);
			assets->createHud(Hud.name, Vector2D(Hud.posx, Hud.posy), Hud.sizex, Hud.sizey, Hud.type, Hud.frames);
		}
	} else {
		std::cout << "Could not load HUD :(" << std::endl;
	}
	//SDL_RenderSetScale(Game::renderer, 1.5, 1.5);
}

//list of all entities for updating
auto& tiles(manager.getGroup(Game::groupMap));
auto& players(manager.getGroup(Game::groupPlayers));
auto& colliders(manager.getGroup(Game::groupColliders));
auto& projectiles(manager.getGroup(Game::groupProjectiles));
auto& texts(manager.getGroup(Game::groupTexts));
auto& enemies(manager.getGroup(Game::groupEnemies));
auto& hudElements(manager.getGroup(Game::groupHUD));
auto& spells(manager.getGroup(Game::groupSpells));

void Game::handleEvents() {

	while (SDL_PollEvent(&event)) {
		if (scenes->getCurrentScene() == 0) {
			keyboardController.keyInput();//player.getComponent<KeyboardController>().keyInput();
			keyboardController.mouseInput();//player.getComponent<KeyboardController>().mouseInput();
		} else {
			keyboardController.keyInputMenu();//player.getComponent<KeyboardController>().keyInputMenu();
			keyboardController.mouseInputMenu();//player.getComponent<KeyboardController>().mouseMenuInput();
		}
	}
}

void Game::update() { 
	
	mTimer->Update();


	//filech neui kollisions funktion

	SDL_Rect playerCol = player.getComponent<ColliderComponent>().collider;


	bool l, r, u, d; //left, right, up, down
	l = keyboardController.walkLeft;
	r = keyboardController.walkRight;
	u = keyboardController.walkUp;
	d = keyboardController.walkDown;

	float sD, sN; //SpeedDiagonal, SpeedNormal
	sD = player.getComponent<PlayerComponent>().getSpeed(true);
	sN = player.getComponent<PlayerComponent>().getSpeed(false);

	if (u){
		playerCol.y -= sN;
		player.getComponent<PlayerComponent>().moveUp();
	} else if (d){
		playerCol.y += sN;
		player.getComponent<PlayerComponent>().moveDown();
	} else {
		player.getComponent<PlayerComponent>().noMoveY();
	}

	/*
	if (d){
		playerCol.y += sN;
	} else if (u) {
		playerCol.y -= sN;
	} 

	if (l) {
		if (d) {
			playerCol.x -= sD;
			playerCol.y += sD;
		} else if (u) {
			playerCol.x -= sD;
			playerCol.y -= sD;
		} else {
			playerCol.x -= sN;
		}
	}

	if (r) {
		if (d) {
			playerCol.x += sD;
			playerCol.y += sD;
		}
		else if (u) {
			playerCol.x += sD;
			playerCol.y -= sD;
		}
		else {
			playerCol.x += sN;
		}
	}
	*/
	
	//check player collider against map collider and resets player position if true
	for (auto& c : colliders) {
		SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;

		/*switch (Collision::AABBx(cCol, playerCol)){
		case 0:
			break;
		case 1:
			while (player.getComponent<TransformComponent>().position.x + playerCol.w + player.getComponent<ColliderComponent>().xOffset >= cCol.x){
				player.getComponent<TransformComponent>().position.x -= 1;
			}
			//player.getComponent<TransformComponent>().velocity.x = 0;

			break;
		case 2:
			while (player.getComponent<TransformComponent>().position.x - playerCol.w - player.getComponent<ColliderComponent>().xOffset <= cCol.x) {
				player.getComponent<TransformComponent>().position.x += 1;
			}

			//player.update();
			break;
		default:
			break;
		}
		
		switch (Collision::AABBy(cCol, playerCol)) {
		case 0:
			break;
		case 1:
			while (player.getComponent<TransformComponent>().position.y - playerCol.h - player.getComponent<ColliderComponent>().yOffset <= cCol.y) {
				player.getComponent<TransformComponent>().position.y += 1;
			}

			//player.update();
			break;
		case 2:
			while (player.getComponent<TransformComponent>().position.y + playerCol.h + player.getComponent<ColliderComponent>().yOffset >= cCol.y) {
				player.getComponent<TransformComponent>().position.y -= 1;
			}


			//player.update();
			break;
		default:
			break;
		}
		

		switch (Collision::AABBxy(cCol, playerCol)){
			case 0:
				break;
			case 1:
				player.getComponent<TransformComponent>().position.x = cCol.x + cCol.w - player.getComponent<ColliderComponent>().xOffset; 			
				break;
			case 3:
				player.getComponent<TransformComponent>().position.x = cCol.x - playerCol.w - player.getComponent<ColliderComponent>().xOffset;
				break;
			case 2:
				player.getComponent<TransformComponent>().position.y = cCol.y + cCol.h - player.getComponent<ColliderComponent>().yOffset;
				break;
			case 4:
				player.getComponent<TransformComponent>().position.y = cCol.y - playerCol.h - player.getComponent<ColliderComponent>().yOffset;
				break;

			default:
				break;
		}
		*/
		
		if (Collision::AABB(cCol, playerCol) == true) {
			if (u) {
				playerCol.y += sN;
			}
			if (d) {
				playerCol.y -= sN;
			}

			player.getComponent<PlayerComponent>().noMoveY();

		}

	}

	if (l) {
		playerCol.x -= sN;
		player.getComponent<PlayerComponent>().moveLeft();
	} else if (r) {
		playerCol.x += sN;
		player.getComponent<PlayerComponent>().moveRight();
	} else {
		player.getComponent<PlayerComponent>().noMoveX();
	}

	for (auto& c : colliders) {
		SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;

		if (Collision::AABB(cCol, playerCol) == true) {
			if (l) {
				playerCol.x += sN;
			}
			
			if (r) {
				playerCol.x -= sN;
			}

			player.getComponent<PlayerComponent>().noMoveX();
		}

	}

	manager.refresh();
	manager.update();

	//collision projectiles
	for (auto& p : projectiles) {
		if (Collision::AABB(player.getComponent<ColliderComponent>().collider, p->getComponent<ColliderComponent>().collider)) {
			//health etc. here
			p->destroy();
		}
	}

	//stamina
	auto& hudStamina(manager.getSubGroup(Game::subHudStamina));
	auto& hudStaminaEnd(manager.getSubGroup(Game::subHudStaminaEnd));
	hudStamina[0]->getComponent<HUDComponent>().updateBarHeight(player.getComponent<PlayerComponent>().getStamina());
	hudStaminaEnd[0]->getComponent<HUDComponent>().updateYPos(hudStamina[0]->getComponent<HUDComponent>().transform->position.y + player.getComponent<PlayerComponent>().getStamina());
	
	//mana
	auto& hudMana(manager.getSubGroup(Game::subHudMana));
	auto& hudManaEnd(manager.getSubGroup(Game::subHudManaEnd));
	hudMana[0]->getComponent<HUDComponent>().updateBarHeight(player.getComponent<PlayerComponent>().getMana());
	hudManaEnd[0]->getComponent<HUDComponent>().updateYPos(hudMana[0]->getComponent<HUDComponent>().transform->position.y + player.getComponent<PlayerComponent>().getMana());

	//health
	auto& hudHealth(manager.getSubGroup(Game::subHudHealth));
	auto& hudHealthEnd(manager.getSubGroup(Game::subHudHealthEnd));
	hudHealth[0]->getComponent<HUDComponent>().updateBarHeight(player.getComponent<PlayerComponent>().getHealth());
	hudHealthEnd[0]->getComponent<HUDComponent>().updateYPos(hudHealth[0]->getComponent<HUDComponent>().transform->position.y + player.getComponent<PlayerComponent>().getHealth());

	//umdräie. mi cha shit ga hole du huere mongoloid
	spellHandler->updatePlayerPos(int(player.getComponent<TransformComponent>().position.x), int(player.getComponent<TransformComponent>().position.y));

	Vector2DInt playerPos = player.getComponent<TransformComponent>().position;


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

	int pPosX = player.getComponent<TransformComponent>().position.y + player.getComponent<TransformComponent>().height;
	for (int i = 0; i < enemies.size(); i++) { //for each T in tiles
		enemies[i]->getComponent<SpriteComponent>().drawRefPlayer(pPosX, true);
	}

	for (auto& p : players) { //for each T in tiles
		p->draw();
	}

	for (int i = 0; i < enemies.size(); i++) { //for each T in tiles
		enemies[i]->getComponent<SpriteComponent>().drawRefPlayer(pPosX, false);
	}


	for (auto& p : projectiles) {
		p->draw();
	}

	for (auto& s : spells) {
		s->draw();
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
		scene = 1;
		scenes->setNewScene(scene);
	} 
	
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
	
	manager.refresh();
}

void Game::newScreen(Vector2DInt pp) {

	Vector2DInt playerPos = pp;
	const char* direction;

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
	//West
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
	//Action (Boat, Boss, Event, Hole, start etc)
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
	std::cout << player.getComponent<TransformComponent>().scale << std::endl;
	
	camera->updateCameraSize(map->getMapSize(), WINDOW_WIDTH, WINDOW_HEIGHT);
}

void Game::clean() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	std::cout << "Game Cleaned" << std::endl;
}