#pragma once
#include "ECS.hpp"
#include "Components.hpp"
#include <iostream>

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

	void updateHealth(int hp) {
		if (hudType == "health"){
			health = hp;
		}
	}

	void updateMana(int mna) {
		if (hudType == "mana"){
			mana = mna;
		}
	}

	void updateStamina(int stm) {
		if (hudType == "stamina"){
			transform->width = stm;
		}
	}

	void updateMaxHealth(int hp) {
		if (hudType == "health_bar") {
			health = hp;
		}
	}

	void updateMaxMana(int mna) {
		if (hudType == "mana_bar") {
			mana = mna;
		}
	}

	void updateMaxStamina(int stm) {
		if (hudType == "stamina_bar") {
			transform->width = stm;
		}
	}

private:
	std::string hudType;
	
	int health, maxHealth;
	int mana, maxMana;
	int stamina, maxStamina;

};