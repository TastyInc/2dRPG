#pragma once
#include "ECS.hpp"
#include "Components.hpp"

//Component that character/enemy stats, items etc
class PlayerComponent : public Component {
public:

	//init enemy etc.
	PlayerComponent(int id) {
		if (id == 1) {
			

		}
	}

	//init player
	PlayerComponent(int health, int stamina, int mna) {
		hp = maxHp = health;
		stm = maxStm = stamina;
		mana = maxMana = mna;
	}

	void updateHealth(int damage) {
		hp = hp - (damage * (damage / (damage + armor)));
	}

	void updateStamina(bool sprinting) {
		if (sprinting) {
			if (stm <= 0) {
				stm = 0;
			}
			else {
				stm -= 1;
			}
		} else {
			if (stm >= maxStm) {
				stm = maxStm;
			} else {
				stm += 1;
			}
		}
	}

	int getStamina() {
		return stm;
	}

	int getMana() {
		return mana;
	}

	int getHealth() {
		return hp;
	}

private:
	int hp;		//healthpoints
	int maxHp;	//max healthpoints
	int armor;	//armor
	int stm;	//stamina
	int maxStm;	//max Stamina
	int mana;	//mana
	int maxMana;//Max mana

	int sizeX, sizeY;

	//items
};