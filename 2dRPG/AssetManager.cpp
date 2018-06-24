#include "AssetManager.hpp"
#include "Components.hpp"

AssetManager::AssetManager(Manager* man) : manager(man) {
	
}

AssetManager::~AssetManager() {

}

void AssetManager::createProjectile(Vector2D pos, Vector2D vel, int range, int speed, std::string id){
	auto& projectile(manager->addEntity());				// Placeholder

	projectile.addComponent<TransformComponent>(pos.x, pos.y, 32, 32, 1);
	projectile.addComponent<SpriteComponent>(id, false);
	projectile.addComponent<ProjectileComponent>(range, speed, vel);
	projectile.addComponent<ColliderComponent>("projectile");
	projectile.addGroup(Game::groupProjectiles);
}

void AssetManager::createText(std::string text, Vector2D pos, std::string font, SDL_Color& color){
	auto& gameText(manager->addEntity());

	gameText.addComponent<UILable>(pos.x, pos.y, text, font, color);
	gameText.addGroup(Game::groupTexts);
}

void AssetManager::createEnemy(int id, Vector2D pos) {
	auto& enemy(manager->addEntity());

	enemy.addComponent<TransformComponent>(pos.x, pos.y, 80, 110, 3);
	enemy.addComponent<SpriteComponent>("enemy", true);
	enemy.addComponent<CharacterComponent>(id);
	enemy.addGroup(Game::groupEnemies);
}

void AssetManager::createHud(std::string id, Vector2D pos, int w, int h){
	auto& hudElement(manager->addEntity());

	hudElement.addComponent<TransformComponent>(pos.x, pos.y, w, h, 1);
	hudElement.addComponent<SpriteComponent>(id);
	hudElement.addGroup(Game::groupHUD);
}

void AssetManager::AddTexture(std::string id, const char* path) {
	textures.emplace(id, TextureManager::LoadTexture(path));
}

SDL_Texture* AssetManager::GetTexture(std::string id) {
	return textures[id];
}

void AssetManager::AddFont(std::string id, std::string path, int fontSize) {
	fonts.emplace(id, TTF_OpenFont(path.c_str(), fontSize));

}

TTF_Font* AssetManager::GetFont(std::string id) {
	return fonts[id];
}