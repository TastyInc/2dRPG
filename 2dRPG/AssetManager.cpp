#include "AssetManager.hpp"
#include "Components.hpp"

AssetManager::AssetManager(Manager* man) : manager(man) {
	
}

AssetManager::~AssetManager() {

}

//PROJECTILE
void AssetManager::createProjectile(Vector2D pos, Vector2D vel, int range, int speed, std::string id){
	auto& projectile(manager->addEntity());				// Placeholder

	projectile.addComponent<TransformComponent>(pos.x, pos.y, 32, 32, 1);
	projectile.addComponent<SpriteComponent>(id, false);
	projectile.addComponent<ProjectileComponent>(range, speed, vel);
	projectile.addComponent<ColliderComponent>("projectile");
	projectile.addGroup(Game::groupProjectiles);
}

//TEXT
void AssetManager::createText(std::string text, Vector2D pos, std::string font, SDL_Color& color){
	auto& gameText(manager->addEntity());

	gameText.addComponent<UILable>(pos.x, pos.y, text, font, color);
	gameText.addGroup(Game::groupTexts);
}

//ENEMY
void AssetManager::createEnemy(int id, Vector2D pos, std::string name) {
	auto& enemy(manager->addEntity());

	enemy.addComponent<TransformComponent>(pos.x, pos.y, 80, 110, 3);
	enemy.addComponent<SpriteComponent>(name, true);
	enemy.addComponent<PlayerComponent>(id);
	enemy.addGroup(Game::groupEnemies);
}

//HUD
void AssetManager::createHud(std::string id, Vector2D pos, int w, int h, const char* type){
	auto& hudElement(manager->addEntity());

	hudElement.addComponent<TransformComponent>(pos.x, pos.y, w, h, 1.0f);
	hudElement.addComponent<SpriteComponent>(id);
	hudElement.addComponent<HUDComponent>(type);
	hudElement.getComponent<HUDComponent>().setType();
	hudElement.addGroup(Game::groupHUD);
}

void AssetManager::AddTexture(std::string id, const char* path) {
	textures.emplace(id, TextureManager::LoadTexture(path));
}

SDL_Texture* AssetManager::GetTexture(std::string id) {
	if (textures[id] != NULL){
		return textures[id];
	}
	
	return NULL;
}

void AssetManager::AddFont(std::string id, std::string path, int fontSize) {
	fonts.emplace(id, TTF_OpenFont(path.c_str(), fontSize));

}

TTF_Font* AssetManager::GetFont(std::string id) {
	return fonts[id];
}