#pragma once
#include <string>
#include "tinyxml2.h"
#include "AssetManager.hpp"

using namespace tinyxml2;

class Map {
public:

	Map();
	~Map();

	void LoadMap(int mtID, int mcID);
	void AddTile(int srcX, int srcY, int xpos, int ypos);

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

	XMLDocument mapFile;
	XMLError eResult;
	XMLElement* mapTheme;
	XMLElement* mapScreen;
	XMLElement* mapType;
	XMLElement* mapAttributes;
};