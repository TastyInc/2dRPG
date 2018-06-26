#include "game.hpp"

Game *game = nullptr;

int main(int argc, const char * argv[]) {
	
	const int FPS = 60;
	const int frameDelay = 1000 / FPS;

	Uint32 frameStart;
	int frameTime;

	game = new Game();
	game->init("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, game->WINDOW_WIDTH, game->WINDOW_HEIGHT, false);
	game->newScene();

	while (game->running()) {

		frameStart = SDL_GetTicks();


		game->handleEvents();
		
		if (Game::scenes->getNewScene() != Game::scenes->getCurrentScene()){
			game->newScene();
		}

		switch (Game::scenes->getCurrentScene()){
		case 0:
			game->update();
			game->render();
			break;
		case 1: 
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
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