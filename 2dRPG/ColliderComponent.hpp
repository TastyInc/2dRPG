#pragma once
#include <string>
#include "SDL.h"
#include "Components.hpp"
#include "TextureManager.hpp"

//Component that handles collision

class ColliderComponent : public Component {
public:
	SDL_Rect collider;
	std::string tag;

	SDL_Texture* tex;
	SDL_Rect srcR, destR;

	TransformComponent* transform;

	bool fixedSize = false;
	int xOffset = 0;
	int yOffset = 0;

	ColliderComponent(std::string t) {
		tag = t;
	}

	ColliderComponent(std::string t, int xpos, int ypos, int size) {
		tag = t;
		collider.x = xpos;
		collider.y = ypos;
		collider.h = collider.w = size;
	}

	//player, enemies etc. with collider != sprite
	ColliderComponent(std::string t, int xOff, int yOff, int w, int h, bool fS) {
		tag = t;
		xOffset = xOff;
		yOffset = yOff;
		collider.h = h;
		collider.w = w;
		fixedSize = fS;
	}

	void init() override {
		if (!entity->hasComponent<TransformComponent>()) {
			entity->addComponent<TransformComponent>();
		}

		transform = &entity->getComponent<TransformComponent>();

		tex = TextureManager::LoadTexture("assets/coltex.png");
		srcR = { 0, 0, 32, 32 };
		destR = { collider.x, collider.y, collider.w, collider.h };

	}

	void update() override {

		//Collider only works on layers "above" the terrain
		if (tag != "terrain") {
			collider.x = static_cast<int>(transform->position.x) + xOffset;
			collider.y = static_cast<int>(transform->position.y) + yOffset;

			if (!fixedSize) {
				collider.w = transform->width * transform->scale;
				collider.h = transform->height * transform->scale;
			}
		}

		destR.x = collider.x - Game::camera.x;
		destR.y = collider.y - Game::camera.y;
	}

	void draw() override {
		TextureManager::Draw(tex, srcR, destR, SDL_FLIP_NONE);
	}

};