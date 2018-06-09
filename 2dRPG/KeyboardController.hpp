#pragma once

#include "game.hpp"
#include "ECS.hpp"
#include "Components.hpp"

class KeyboardController : public Component {
public:
	TransformComponent *transform;
	SpriteComponent *sprite;

	bool wPressed = false;
	bool aPressed = false;
	bool sPressed = false;
	bool dPressed = false;

	void init() override {
		transform = &entity->getComponent<TransformComponent>();
		sprite = &entity->getComponent<SpriteComponent>();
	}

	void update() override {

		if (Game::event.type == SDL_KEYDOWN) {
			switch (Game::event.key.keysym.sym)	{
			case SDLK_w:
				wPressed = true;
				break;
			case SDLK_s:
				sPressed = true;
				break;
			case SDLK_a:
				aPressed = true;
				break;
			case SDLK_d:
				dPressed = true;
				break;
			default:
				break;
			}
		}

		if (Game::event.type == SDL_KEYUP) {
			switch (Game::event.key.keysym.sym) {
			case SDLK_w:
				wPressed = false;
				break;
			case SDLK_s:
				sPressed = false;
				break;
			case SDLK_a:
				aPressed = false;
				break;
			case SDLK_d:
				dPressed = false;
				break;
			case SDLK_ESCAPE:
				Game::isRunning = false;
				break;
			default:
				break;
			}
		}

		if (dPressed || aPressed || sPressed || wPressed) {
			sprite->Play("Walk");

			if (wPressed) {
				transform->velocity.y = -1;
			}

			if (sPressed) {
				transform->velocity.y = 1;
			}

			if (aPressed) {
				transform->velocity.x = -1;
			}

			if (dPressed) {
				transform->velocity.x = 1;
			}
		} else {
			sprite->Play("Idle");
		}

		if (dPressed == false && aPressed == false) {
			transform->velocity.x = 0;
		}

		if (wPressed == false && sPressed == false) {
			transform->velocity.y = 0;
		}
	}
};