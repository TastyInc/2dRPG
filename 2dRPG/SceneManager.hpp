#pragma once
#include "SDL.h"

class SceneManager {
public:
	SceneManager() {
		currentScene = 1;
	}
	~SceneManager() {}

	void setScene(int scene) {
		currentScene = scene;
	}
	
	int getScene(std::string id) {
		//return scene
	}
	
	int getCurrentScene() {
		return currentScene;
	}

private:
	int currentScene;
};

class MenuHandler {
private:

public:
	MenuHandler() {}
	~MenuHandler() {}

};
