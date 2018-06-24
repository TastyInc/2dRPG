#pragma once

//Component that character/enemy stats, items etc
class CharacterComponent : public Component {
public:

	//init enemy etc.
	CharacterComponent(int id) {
		if (id == 1) {
			

		}
	
	}

	//init player
	CharacterComponent(int health, int stamina) {
		hp = health;
		stm = stamina;
	}

	void updateHealth(int damage) {
		hp = hp - (damage * (damage / (damage + armor)));
	}

	void updateStamina(int lostStamina) {
		if (stm - lostStamina <= 0) {
			stm = 0;
		} else {
			stm -= lostStamina;
		}
	}

private:
	int hp;		//healthpoints
	int armor;	//armor
	int stm;	//stamina

	int sizeX, sizeY;

	//items
};