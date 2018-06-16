#include "game.hpp"

Game *game = nullptr;

int main(int argc, const char * argv[]) {
	
	const int FPS = 60;
	const int frameDelay = 1000 / FPS;

	Uint32 frameStart;
	int frameTime;

	int scene;

	game = new Game();
	game->init("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, game->WINDOW_WIDTH, game->WINDOW_HEIGHT, false);

	while (game->running()) {

		frameStart = SDL_GetTicks();

		scene = Game::scenes->getCurrentScene();

		game->handleEvents();

		switch (scene){
		case 0:
			game->update();
			game->render();
			break;
		case 1: 
			game->renderMenu();
			break;
		default:
			break;
		}


		frameTime = SDL_GetTicks() - frameStart;

		if (frameDelay > frameTime) {
			SDL_Delay(frameDelay - frameTime);
		}

	}

	game->clean();
	
	return 0;
}