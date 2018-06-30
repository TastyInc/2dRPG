#pragma once
#include "SDL.h"
#include "game.hpp"
#include "ECS.hpp"
#include "Components.hpp"
#include "Vector2D.hpp"

class SpellComponent : public Component {
public:
	//SpriteComponent* sprite;


	SpellComponent(int dmg, int mana, float destTime) {
		sDamage = dmg;
		sMana = mana;
		destroyDeltaTime = destTime;
	}

	~SpellComponent() {
	
	}

	void update() override {

		//if (Game::spellHandler->deltaButton > 0){
			//entity->getComponent<ProjectileComponent>().releaseProjectile(2, Vector2D(1, 1));

		//}
	}

private:
	int mouseX, mouseY;
	int sDamage;
	int sMana;
	float destroyDeltaTime;

};