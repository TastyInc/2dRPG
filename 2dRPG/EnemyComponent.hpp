#pragma once
#include "ECS.hpp"
#include "Components.hpp"
#include <stdlib.h>
#include <ctime>
#include <cmath>
#include "Timer.hpp"

//Component that character/enemy stats, items etc
class EnemyComponent : public Component {
public:

	//init enemy etc.
	EnemyComponent() {
		srand(time(NULL));
		eTimer = Timer::Instance();
	}

	void init() override {
		transform = &entity->getComponent<TransformComponent>();
		sprite = &entity->getComponent<SpriteComponent>();

		endMoveX = spawnX = int(transform->position.x);
		endMoveY = spawnY = int(transform->position.y);
	
	}

	void update() override {

		int posX = transform->position.x += moveX;
		int posY = transform->position.y += moveY;

		if ((posX >= endMoveX && moveX >= 0) || (posX <= endMoveX && moveX <= 0)){

			moveX = 0;
			moveY = 0;

			if (eTimer->DeltaTime() > pauseAfterMove) {
				
				moveRandom();
				eTimer->Reset();
			}
		} else {
			eTimer->Reset();
		}
	}

	void moveRandom() {

		int posX = transform->position.x;
		int posY = transform->position.y;

		do{
			endMoveX = posX + (rand() % maxDistanceToSpawn - (maxDistanceToSpawn / 2));;
		} while (endMoveX < spawnX - maxDistanceToSpawn || endMoveX > spawnX + maxDistanceToSpawn);

		do {
			endMoveY = posY + (rand() % maxDistanceToSpawn - (maxDistanceToSpawn / 2));;
		} while (endMoveY < spawnY - maxDistanceToSpawn || endMoveY > spawnY + maxDistanceToSpawn);
	
		double angleDist = sqrt(pow(endMoveX - posX, 2.0f) + pow(endMoveY - posY, 2.0f));

		moveX = std::rint((endMoveX - posX) / angleDist);
		moveY = std::rint((endMoveY - posY) / angleDist);	
	}

	void moveToPlayer() {
	
	
	}


private:
	TransformComponent * transform;
	SpriteComponent *sprite;

	int spawnX, spawnY;
	int maxDistanceToSpawn = 250;
	int endMoveX, endMoveY;
	float moveSpeed = 0.2f;
	int moveX, moveY;
	float runSpeed;
	float pauseAfterMove = 2.0f;

	Timer* eTimer;

protected:


};