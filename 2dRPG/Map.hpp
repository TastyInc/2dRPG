#pragma once
#include <string>
#include "tinyxml2.h"
#include "AssetManager.hpp"
#include "Vector2D.hpp"

using namespace tinyxml2;

class Map {
public:

	Map();
	~Map();

	void LoadMap(int mtID, int mcID);
	void AddTile(int srcX, int srcY, int xpos, int ypos);
	Vector2D getMapSize();

private:
	std::string texID;
	std::string tilemapS;
	const char* tileset;
	int mapScale;
	int tileSize;
	int scaledSize;
	int mapThemeID = 0;
	int mapScreenID = 0;
	int tilesX, tilesY;
	Vector2D mapSize;

	XMLDocument mapFile;
	XMLError eResult;
	XMLElement* mapTheme;
	XMLElement* mapScreen;
	XMLElement* mapType;
	XMLElement* mapAttributes;
};