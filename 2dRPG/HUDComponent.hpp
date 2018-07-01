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
			stamina = stm;
			transform->width = stm;
		}
	}

private:
	std::string hudType;
	
	int health;
	int mana;
	int stamina;


};