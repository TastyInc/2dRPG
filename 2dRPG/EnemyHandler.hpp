#pragma once
#include "ECS.hpp"
#include "Components.hpp"
#include "game.hpp"

extern Manager manager;

class EnemyHandler{
public:
	EnemyHandler() {};

	~EnemyHandler() {};

	void loadEnemy() {
	
	
	}

	void createEnemy() {
		
		auto& enemy(manager.addEntity());

		enemy.addComponent<TransformComponent>(2000, 2000, 64, 72, 2);
		enemy.addComponent<SpriteComponent>("golem.png", 8, 120, 0, "enemy");
		enemy.addComponent<EnemyComponent>();
		enemy.addGroup(Game::groupEnemies);
	}

	void destroyEnemy() {
		
	
	}


private:


};