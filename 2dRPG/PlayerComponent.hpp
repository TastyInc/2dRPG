#pragma once
#include "ECS.hpp"
#include "Components.hpp"

//Component that character/enemy stats, items etc
class PlayerComponent : public Component {
public:
	Vector2D pVel;


	void init() override {
		transform = &entity->getComponent<TransformComponent>();
	}

	PlayerComponent() {
	
	}

	//init player
	PlayerComponent(int health, int stamina, int mna) {
		hp = maxHp = health;
		stm = maxStm = stamina;
		mana = maxMana = mna;
		pVel = { 0, 0 };


		updateDiagonalSpeed();
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

	void updateDiagonalSpeed(){
		 diagonalSpeed = sqrt(2) * speed / 2; 
	}

	void noMoveX() {
		pVel.x = 0;
	}

	void noMoveY() {
		pVel.y = 0;
	}

	void moveUp() {
		pVel.y = -speed;
	}

	void moveDown() {
		pVel.y = speed;
	}

	void moveLeft() {
		pVel.x = -speed;
	}

	void moveRight() {
		pVel.x = speed;
	}

	float getSpeed(bool diagonal){
		if (diagonal) {
			return diagonalSpeed;
		} else {
			return speed;
		}
	}

	void moving(bool up, bool down, bool left, bool right) {
		if (up == false && down == false && left == false && right == false){
			
		}

		if (up){
			if (left) {
				pVel.y = diagonalSpeed * -1;
				pVel.x = diagonalSpeed * -1;
			} else if (right) {
				pVel.y = diagonalSpeed * -1;
				pVel.x = diagonalSpeed;
			} else {
				pVel.y = speed * -1;
			}
		} 

		if (down) {
			if (left) {
				pVel.y = diagonalSpeed;
				pVel.x = diagonalSpeed * -1;
			} else if (right) {
				pVel.y = diagonalSpeed;
				pVel.x = diagonalSpeed;
			} else {
				pVel.y = speed;
			}
		}

		if (left) {
			if (up) {
				pVel.y = diagonalSpeed * -1;
				pVel.x = diagonalSpeed * -1;
			}
			else if (down) {
				pVel.y = diagonalSpeed;
				pVel.x = diagonalSpeed * -1;
			}else {
				pVel.x = speed * -1;
			}
		}

		if (right) {
			if (up) {
				pVel.y = diagonalSpeed * -1;
				pVel.x = diagonalSpeed;
			}
			else if (down) {
				pVel.y = diagonalSpeed;
				pVel.x = diagonalSpeed;
			}
			else {
				pVel.x = speed;
			}
		}

	}

	void isColliding(bool col) {
		colliding = col;
	}

	void update() override {
		if (!colliding){
			transform->velocity = pVel;
		}
	}

private:
	TransformComponent* transform;

	int hp;		//healthpoints
	int maxHp;	//max healthpoints
	int armor;	//armor
	int stm;	//stamina
	int maxStm;	//max Stamina
	int mana;	//mana
	int maxMana;//Max mana

	bool colliding = false;

	float speed = 1.0f;
	float diagonalSpeed;

	int sizeX, sizeY;

	//items
};