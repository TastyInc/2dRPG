#include "Map.hpp"
#include "game.hpp"
#include "ECS.hpp"
#include "Components.hpp"
#include <string>
#include "AssetManager.hpp"

//Manager Object defined somewhere else
extern Manager manager;

Map::Map() {
}

Map::~Map() {
}

void Map::LoadMap(int eventNum) {

}

void Map::LoadMap(const char* direction) {
	mapScreen->QueryStringAttribute(direction, &newMap);
	std::stringstream ss;
	ss << newMap;
	int newMtID = atoi(ss.str().substr(0, 2).c_str());
	int newMcID = atoi(ss.str().substr(2, 4).c_str());
	
	if (LoadMap(newMtID, newMcID) != 1) {
		std::cout << "could not load new map" << std::endl;
	}
}

int Map::LoadMap(int mtID, int mcID) {

	eResult = mapFile.LoadFile("resources/maps.xml");
	if (eResult != XML_SUCCESS) {
		std::cout << "Could not load Mapfile" << std::endl;
		return 0;
	}

	XMLNode* root = mapFile.FirstChildElement("maps");
	if (root == nullptr) {
		std::cout << "Could not load Maps" << std::endl;
		return 0;
	}

	mapTheme = root->FirstChildElement("maptheme");
	mapTheme->QueryIntAttribute("id", &mapThemeID);

	//loop through mapThemes
	while (mapThemeID != mtID){
		mapTheme = mapTheme->NextSiblingElement("maptheme");
		mapTheme->QueryIntAttribute("id", &mapThemeID);
	}

	if (mapTheme == nullptr) {
		std::cout << "Could not load Maptheme" << std::endl;
		return 0;
	}

	mapScreen = mapTheme->FirstChildElement("mapscreen");
	mapScreen->QueryIntAttribute("id", &mapScreenID);

	//loop through mapScreens
	while (mapScreenID != mcID) {
		mapScreen = mapScreen->NextSiblingElement("mapscreen");
		mapScreen->QueryIntAttribute("id", &mapScreenID);
	}

	if (mapScreen == nullptr) {
		std::cout << "Could not load Mapscreen" << std::endl;
		return 0;
	}

	mapAttributes = mapScreen->FirstChildElement("attributes");

	if (mapAttributes == nullptr) {
		std::cout << "Could not load Map Attributes" << std::endl;
		return 0;
	}

	mapAttributes->QueryIntAttribute("width", &tilesX);
	mapAttributes->QueryIntAttribute("height", &tilesY);
	mapAttributes->QueryIntAttribute("scale", &mapScale);
	mapAttributes->QueryIntAttribute("tilesize", &tileSize);
	tileset = mapAttributes->Attribute("tileset");

	scaledSize = mapScale * tileSize;

	//texture vo de sprites hie
	Game::assets->AddTexture("terrain", tileset);
	
	mapType = mapScreen->FirstChildElement("terrainmap");
	std::string tilemapS = mapType->GetText();

	tilemapS.erase(std::remove(tilemapS.begin(), tilemapS.end(), '\t'), tilemapS.end());
	tilemapS.erase(std::remove(tilemapS.begin(), tilemapS.end(), '\n'), tilemapS.end());

	int srcX, srcY, stringPos = 0;

	for (int y = 0; y < tilesY; y++) {
		for (int x = 0; x < tilesX; x++) {

			srcY = (tilemapS[stringPos] - '0') * tileSize;
			stringPos += 1;
			srcX = (tilemapS[stringPos] - '0') * tileSize;
			stringPos += 1;
			AddTile(srcX, srcY, x * scaledSize, y * scaledSize);

		}
	}

	mapType = mapScreen->FirstChildElement("colmap");
	tilemapS = mapType->GetText();

	tilemapS.erase(std::remove(tilemapS.begin(), tilemapS.end(), '\t'), tilemapS.end());
	tilemapS.erase(std::remove(tilemapS.begin(), tilemapS.end(), '\n'), tilemapS.end());

	stringPos = 0;

	for (int y = 0; y < tilesY; y++) {
		for (int x = 0; x < tilesX; x++) {

			if (tilemapS[stringPos] == '1') {
				auto& tcol(manager.addEntity());
				tcol.addComponent<ColliderComponent>("terrain", x * scaledSize, y * scaledSize, scaledSize);
				tcol.addGroup(Game::groupColliders);
			} else if (tilemapS[stringPos] == '4') {
				spawnPoint = {float( x * scaledSize) ,float( y * scaledSize )};
			}
			stringPos += 1;
		}
	}

	return 1;
	//spritemap
}

Vector2D Map::getMapSize() {
	mapSize = { float(scaledSize * tilesX), float(scaledSize * tilesY) };
	return mapSize;

}

int Map::getMapScreenID() {
	return mapScreenID;
}

int Map::getMapThemeID() {
	return mapThemeID;
}

void Map::AddTile(int srcX, int srcY, int xpos, int ypos) {
	auto& tile(manager.addEntity());
	tile.addComponent<TileComponent>(srcX, srcY, xpos, ypos, tileSize, mapScale, "terrain");
	tile.addGroup(Game::groupMap);
}
