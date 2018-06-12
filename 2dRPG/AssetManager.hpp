#pragma once
#include <map>
#include <string>
#include "TextureManager.hpp"
#include "Vector2D.hpp"
#include "ECS.hpp"

class AssetManager {
public:
	AssetManager(Manager* man);
	~AssetManager();

	//gameobjects
	void createProjectile(Vector2D pos, Vector2D vel, int range, int speed, std::string id);


	//texture management
	void AddTexture(std::string id, const char* path);
	SDL_Texture* GetTexture(std::string id);

private:
	Manager* manager;
	std::map<std::string, SDL_Texture*> textures;
};