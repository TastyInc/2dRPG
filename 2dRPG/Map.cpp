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

void Map::LoadMap(int mtID, int mcID) {

	eResult = mapFile.LoadFile("resources/maps.xml");
	if (eResult != XML_SUCCESS) {
		std::cout << "Could not load Mapfile" << std::endl;
	}

	XMLNode* root = mapFile.FirstChildElement("maps");
	if (root == nullptr) {
		//ERROR
	}

	mapTheme = root->FirstChildElement("maptheme");
	mapTheme->QueryIntAttribute("id", &mapThemeID);

	//loop through mapThemes
	while (mapThemeID != mtID){
		mapTheme = mapTheme->NextSiblingElement("maptheme");
		mapTheme->QueryIntAttribute("id", &mapThemeID);
	}

	if (mapTheme == nullptr) {
		//error
	}

	mapScreen = mapTheme->FirstChildElement("mapscreen");
	mapScreen->QueryIntAttribute("id", &mapScreenID);

	//loop through mapSCreens
	while (mapScreenID != mcID) {
		mapScreen = mapTheme->NextSiblingElement("mapscreen");
		mapScreen->QueryIntAttribute("id", &mapThemeID);
	}

	if (mapScreen == nullptr) {
		//error
	}

	mapAttributes = mapScreen->FirstChildElement("attributes");

	if (mapAttributes == nullptr) {
		//error
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
			}
			stringPos += 1;
		}
	}

	//spritemap
}

void Map::AddTile(int srcX, int srcY, int xpos, int ypos) {
	auto& tile(manager.addEntity());
	tile.addComponent<TileComponent>(srcX, srcY, xpos, ypos, tileSize, mapScale, "terrain");
	tile.addGroup(Game::groupMap);
}
