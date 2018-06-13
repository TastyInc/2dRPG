#pragma once

#include "Components.hpp"
#include "Map.hpp"

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


};

