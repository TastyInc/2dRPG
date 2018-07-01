#pragma once
#include "Components.hpp"
#include "TextureManager.hpp"
#include "SDL.h"
#include "animation.hpp"
#include <map>
#include "AssetManager.hpp"

class SpriteComponent: public Component {
private:
	TransformComponent *transform;
	SDL_Texture *texture;
	SDL_Rect srcRect, destRect;

	bool animated = false;
	int frames = 0;
	int speed = 100;
	
public:
	int animIndex = 0;

	std::map<const char*, Animation> animations;

	SpriteComponent() = default;

	SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;

	SpriteComponent(std::string id) {
		setTex(id);
	}

	//F�r player
	SpriteComponent(std::string id, bool isAnimated) {
		animated = isAnimated;

		//anpassen f�r enemies angeri animations u angeri framerate etc
		Animation idle = Animation(0, 4, 200);
		Animation walkUp = Animation(1, 8, 100);
		Animation walkDown = Animation(2, 8, 100);
		Animation walkLR = Animation(3, 8, 100);
		Animation sprintUp = Animation(1, 8, 60);
		Animation sprintDown = Animation(2, 8, 60);
		Animation sprintLR = Animation(3, 8, 60);

		animations.emplace("Idle", idle);
		animations.emplace("WalkUp", walkUp);
		animations.emplace("WalkDown", walkDown);
		animations.emplace("WalkLR", walkLR);
		animations.emplace("SprintUp", sprintUp);
		animations.emplace("SprintDown", sprintDown);
		animations.emplace("SprintLR", sprintLR);

		Play("Idle");

		setTex(id);
	}

	//for spells
	SpriteComponent(std::string id, int frames, int delay, int index, const char* spellName) {
		animated = true;

		Animation spell = Animation(index, frames, delay);
		animations.emplace(spellName, spell);

		Play(spellName);

		setTex(id);
	}

	void addAnimation(int frames, int delay, int index, const char* spellName) {
		Animation spell = Animation(index, frames, delay);
		animations.emplace(spellName, spell);
	}
	
	~SpriteComponent() {
		
	}

	void setTex(std::string id) {
		texture = Game::assets->GetTexture(id);
	}

	void init() override {
		
		transform = &entity->getComponent<TransformComponent>();
		
		srcRect.x = srcRect.y = 0;
		srcRect.w = transform->width;
		srcRect.h = transform->height;

	}

	void update() override {

		if (animated) {
			srcRect.x = srcRect.w * static_cast<int>((SDL_GetTicks() / speed) % frames);
		}

		srcRect.y = animIndex * transform->height;

		//switch f�r hud, tilemap, background etc.
		if (entity->hasGroup(Game::groupHUD)) {
			destRect.x = static_cast<int>(transform->position.x);
			destRect.y = static_cast<int>(transform->position.y);
		} else {
			destRect.x = static_cast<int>(transform->position.x) - Game::camera->camera.x;
			destRect.y = static_cast<int>(transform->position.y) - Game::camera->camera.y;
		}

		destRect.w = static_cast<int>(transform->width * transform->scale);
		destRect.h = static_cast<int>(transform->height * transform->scale);
	}

	void draw() override {
	
		TextureManager::Draw(texture, srcRect, destRect, spriteFlip);
	}

	void Play(const char* animName) {
		frames = animations[animName].frames;
		animIndex = animations[animName].index;
		speed = animations[animName].speed;
	}
};