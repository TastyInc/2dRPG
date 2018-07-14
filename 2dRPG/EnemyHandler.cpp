#include "EnemyHandler.hpp"
#include "Components.hpp"
#include "game.hpp"

EnemyHandler::EnemyHandler() {


}

void EnemyHandler::createEnemy(){
	
	Game::assets->AddTexture("Golem", "resources/sprites/enemies/golem.png");

	auto& enemy(manager.addEntity());

	enemy.addComponent<TransformComponent>(800, 800, 64, 72, 2);
	enemy.addComponent<SpriteComponent>("Golem", 7, 120, 1, "enemy");
	enemy.addComponent<EnemyComponent>();
	enemy.addGroup(Game::groupEnemies);
}