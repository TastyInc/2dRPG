#pragma once
#include "ECS.hpp"
#include "Components.hpp"
#include <iostream>
#include "game.hpp"

class HUDComponent : public Component {
public:
	TransformComponent* transform;


	HUDComponent(const char * type) {
		if (type != NULL) {
			hudType = type;

		}
	}

	void setType() {
	
		if (!hudType.empty()) {

			if (hudType == "health") {
				entity->addSubGroup(Game::subHudHealth);
			}
			else if (hudType == "mana") {
				entity->addSubGroup(Game::subHudMana);
			}
			else if (hudType == "stamina") {
				entity->addSubGroup(Game::subHudStamina);
			}
			else if (hudType == "stamina_bottom") {
				entity->addSubGroup(Game::subHudStaminaEnd);
			}
			else if (hudType == "health_bottom") {
				entity->addSubGroup(Game::subHudHealthEnd);
			}
			else if (hudType == "mana_bottom") {
				entity->addSubGroup(Game::subHudManaEnd);
			}
		}
	}

	~HUDComponent() {

	}

	void update() override {

	}

	void init() override {
		if (entity->hasComponent<TransformComponent>()) {
			transform = &entity->getComponent<TransformComponent>();
		}
	}

	void updateBarHeight(int h) {
		transform->height = h;
	}

	void updateEndPos(int posY) {
		transform->position.y = posY;
	}


private:
	std::string hudType;
	
	int health, maxHealth;
	int mana, maxMana;
	int stamina, maxStamina;

};