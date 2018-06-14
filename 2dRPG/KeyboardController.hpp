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
	}

	void setVelocity(int velX, int velY) {
		transform->velocity.x = float(velX);
		transform->velocity.y = float(velY);
	}

	void update() override {
		/*
		if (Game::event.type == SDL_KEYDOWN) {
			switch (Game::event.key.keysym.sym)	{
			case SDLK_w:
				transform->velocity.y = -1;
				sprite->Play("WalkUp");
				break;
			case SDLK_s:
				transform->velocity.y = 1;
				break;
			case SDLK_a:
				transform->velocity.x = -1;
				break;
			case SDLK_d:
				transform->velocity.x = 1;
				break;
			case SDLK_o:
				Game::scene->setScene(2);
			default:
				break;
			}
		}

		if (Game::event.type == SDL_KEYUP) {
			switch (Game::event.key.keysym.sym) {
			case SDLK_w:
			case SDLK_s:
				transform->velocity.y = 0;
				break;
			case SDLK_a:
			case SDLK_d:
				transform->velocity.x = 0;
				break;
			case SDLK_ESCAPE:
				Game::isRunning = false;
				break;
			default:
				break;
			}
		}*/

		if (transform->velocity.x == 0 && transform->velocity.y == 0) {
			sprite->Play("Idle");
		} else {
			if (transform->velocity.x > 0) {
				sprite->spriteFlip = SDL_FLIP_NONE;
				sprite->Play("WalkLR");
			}

			if (transform->velocity.x < 0) {
				sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
				sprite->Play("WalkLR");
			}

			if (transform->velocity.y > 0) {
				sprite->Play("WalkDown");
			}

			if (transform->velocity.y < 0) {
				sprite->Play("WalkUp");
			}
		}
	}
};