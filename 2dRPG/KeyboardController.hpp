#pragma once

#include "game.hpp"
#include "ECS.hpp"
#include "Components.hpp"

class KeyboardController : public Component {
public:
	TransformComponent *transform;
	SpriteComponent *sprite;

	void init() override {
		transform = &entity->getComponent<TransformComponent>();
		sprite = &entity->getComponent<SpriteComponent>();
		playerVel.Zero();
		walkingX = 0;
		walkingY = 0;

		kTimer = Timer::Instance();
	}

	void setVelocity() {
		if (pSprint) {
			playerVel.x = sprintSpeed * walkingX;
			playerVel.y = sprintSpeed * walkingY;
		} else {
			playerVel.x = walkingX;
			playerVel.y = walkingY;
		}

		transform->velocity.x = playerVel.x;
		transform->velocity.y = playerVel.y;
	}

	bool isSprinting() {
		return pSprint;
	}

	void keyInput() {
		switch (Game::event.type) {
		case SDL_QUIT:
			Game::isRunning = false;
			break;
		case SDL_KEYDOWN:
			switch (Game::event.key.keysym.sym) {
			case SDLK_LSHIFT:
				pSprint = true;
				break;
			case SDLK_w:
				walkingY = -walkSpeed;
				break;
			case SDLK_s:
				walkingY = walkSpeed;
				break;
			case SDLK_a:
				walkingX = -walkSpeed;
				break;
			case SDLK_d:
				walkingX = walkSpeed;
				break;
				break;
			default:
				break;
			}
		}

		switch (Game::event.type) {
			case SDL_KEYUP:
				switch (Game::event.key.keysym.sym) {
				case SDLK_LSHIFT:
					pSprint = false;
					break;
				case SDLK_w:
				case SDLK_s:
					walkingY = 0;
					break;
				case SDLK_a:
					if (walkingX != walkSpeed) {
						walkingX = 0;
					}
					break;
				case SDLK_d:
					if (walkingX != -walkSpeed) {
						walkingX = 0;
					}
					break;
				case SDLK_ESCAPE:
					Game::scenes->setNewScene(2);
					break;
				default:
					break;
				}
			default:
				break;
		}
	}

	void mouseInput() {
		kTimer->Update();
		if (Game::event.button.button == SDL_BUTTON_LEFT) {
			if (Game::event.type == SDL_MOUSEBUTTONDOWN) {
				//std::cout << mouseX << " " << mouseY << std::endl;
				kTimer->Reset();
			}
			else if (Game::event.type == SDL_MOUSEBUTTONUP) {
				SDL_GetMouseState(&mouseX, &mouseY);
				//std::cout << kTimer->DeltaTime() << std::endl;
				Game::spellHandler->LoadSpell(1, kTimer->DeltaTime(), mouseX, mouseY);
				kTimer->Reset();
			}
		}
	}

	void mouseMenuInput() {
		if (Game::event.type == SDL_MOUSEBUTTONDOWN) {

		}
	}

	void keyInputMenu() {
		switch (Game::event.type) {
		case SDL_KEYDOWN:
			switch (Game::event.key.keysym.sym) {
			case SDLK_w:
				Game::scenes->menus->buttonUpdate(-1);
				break;
			case SDLK_s:
				Game::scenes->menus->buttonUpdate(1);
				break;
			case SDLK_RETURN:
			case SDLK_KP_ENTER:
			case SDLK_e:
				Game::scenes->menus->buttonPressed();
				break;
			default:
				break;
			}
		}
	}

	void update() override {
		
		if (transform->velocity.x == 0 && transform->velocity.y == 0) {
			sprite->Play("Idle");
		} else {
			if (transform->velocity.x > 0) 
				sprite->spriteFlip = SDL_FLIP_NONE;

			if (transform->velocity.x < 0) 
				sprite->spriteFlip = SDL_FLIP_HORIZONTAL;

			if (transform->velocity.x != 0) {
				if (pSprint) {
					sprite->Play("SprintLR");
				}
				else {
					sprite->Play("WalkLR");
				}
			}

			if (transform->velocity.y > 0) {
				if (pSprint) {
					sprite->Play("SprintDown");
				} else {
					sprite->Play("WalkDown");
				}
			}

			if (transform->velocity.y < 0) {
				if (pSprint) {
					sprite->Play("SprintUp");
				}
				else {
					sprite->Play("WalkUp");
				}
			}
		}
	}

	int getMouseX() {
		return mouseX;
	}

	int getMouseY() {
		return mouseY;
	}

	float getDeltaMousePress(){
		return kTimer->DeltaTime();
	}

private:
	Vector2D playerVel;
	float walkSpeed = 1;
	float sprintSpeed = 2;

	float walkingX;
	float walkingY;

	bool pSprint = false;
	bool pMoving = false;

	Timer* kTimer;

	int mouseX, mouseY;

};