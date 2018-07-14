#pragma once
#include <map>
#include <string>
#include "TextureManager.hpp"
#include "Vector2D.hpp"
#include "ECS.hpp"
#include "SDL_ttf.h"

class AssetManager {
public:
	AssetManager(Manager* man);
	~AssetManager();

	//gameobjects
	void createProjectile(Vector2D pos, Vector2D vel, int range, int speed, std::string id);
	void createText(std::string text, Vector2D pos, std::string font, SDL_Color& color);
	void createEnemy(int id, Vector2D pos, std::string name);
	void createHud(std::string id, Vector2D pos, int w, int h, const char* type, int frames);

	//texture management
	void AddTexture(std::string id, const char* path);
	SDL_Texture* GetTexture(std::string id);

	//fonts
	void AddFont(std::string id, std::string path, int fontSize);
	TTF_Font* GetFont(std::string id);

private:
	Manager* manager;
	std::map<std::string, SDL_Texture*> textures;
	std::map<std::string, TTF_Font*> fonts;
	std::map<std::string, Vector2D> texts;
};