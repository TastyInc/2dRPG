#pragma once

#include "Components.hpp"
#include "Map.hpp"
#include <vector>

class LevelManager {
public:
	LevelManager() {
	
	}

	~LevelManager() {
	
	}

	void LoadLevel(int id) {
		level = new Map("", 2, 2);
	}

	void SetLevel() {
	
	}

	int GetLevel() {
	
	}

private:
	Map* level;
};

class Level {
public:

private:
	std::vector<int, int> levelID = { 1,2,3,4,5 };
	std::vector<std::string> levelName[]		= {"Beach", "Melancholy Hills", "Gabelmeisters Peak"};
	int levelWidth[] = {1, 2, 3};
	int levelHeight[] = {};

};

