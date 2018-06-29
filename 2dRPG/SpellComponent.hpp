#pragma once
#include "ECS.hpp"
#include "Components.hpp"
#include "Vector2D.hpp"

class SpellComponent : public Component {
public:
	//SpriteComponent* sprite;


	SpellComponent(int dmg, int mana) {
		sDamage = dmg;
		sMana = mana;
	}

	~SpellComponent() {
	
	}

	float getDestroyTime() {
		return destroyDeltaTime;
	}

private:
	int sDamage;
	int sMana;
	float destroyDeltaTime;

};