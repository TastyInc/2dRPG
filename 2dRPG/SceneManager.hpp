#pragma once
#include "tinyxml2.h"

using namespace tinyxml2;

class MenuHandler {
public:
	int eleCount;
	SDL_Rect menuRect[6];
	std::string menuText[6];
	int menuAction[6];
	int activeButton = 1;
	bool btnPressed = false;

	MenuHandler() {}
	~MenuHandler() {}

	/*void draw() {
		for (int i = 0; i < eleCount; i++){
			
		}
	}*/

	bool LoadMenu(int menu) {
		eleCount = 0;

		eResult = menuFile.LoadFile("resources/menus.xml");
		if (eResult != XML_SUCCESS) {
			std::cout << "Could not load MenuFile" << std::endl;
			return false;
		}

		XMLNode* root = menuFile.FirstChildElement("menulist");
		if (root == nullptr) {
			return false;
		}

		selectedMenu = root->FirstChildElement("menu");
		selectedMenu->QueryIntAttribute("id", &menuID);

		//loop through Menu
		while (menuID != menu) {
			selectedMenu = selectedMenu->NextSiblingElement("menu");
			selectedMenu->QueryIntAttribute("id", &menuID);
		}

		for (XMLElement* e = selectedMenu->FirstChildElement("element"); e != NULL;  e = e->NextSiblingElement("element")) {
			menuText[eleCount] = e->Attribute("text");
			e->QueryIntAttribute("action", &menuAction[eleCount]);
			menuRect[eleCount] = { 100, eleCount * 60 + 500, 500, 40 };

			eleCount++;

		}

		return true;
	}

	void buttonUpdate(int i) {
		activeButton = activeButton + i;
		if (activeButton == 0){
			activeButton = eleCount;
		}else if (activeButton == eleCount + 1) {
			activeButton = 1;
		}
	}

	int buttonColor(int i) {
		if (i == activeButton) {
			return 150;
		} else {
			return 0;
		}
	}

	void buttonPressed() {
		btnPressed = true;
	}

private:
	
	XMLDocument menuFile;
	XMLError eResult;
	XMLElement* selectedMenu;
	XMLElement* mapScreen;
	XMLElement* mapTypes;
	XMLElement* mapAttributes;

	int menuID;
};



class SceneManager {
public:
	MenuHandler * menus;

	SceneManager(MenuHandler* mHandler) : menus(mHandler){
		currentScene = 0; //erste Scene. Auf 1 für Hautpmenu
	}

	~SceneManager() {}

	void setScene(int scene) {
		if (scene != 0) {
			if (menus->LoadMenu(scene) == false) {
				std::cout << "Could not change to Menu or Game, god dammit" << std::endl;
			} else {
				
			}
		}
		menus->btnPressed = false;
		currentScene = newScene;
	}

	int getNewScene() {
		if (menus->btnPressed == true) {
			newScene = menus->menuAction[menus->activeButton - 1];
		}
		return newScene;
	}

	int getCurrentScene() {
		return currentScene;
	}

	void setNewScene(int scene) {
		newScene = scene;
	}

	int getSceneElementCount() {
		return menus->eleCount;
	}

	/*void draw() {
		menus->draw();
	}*/

private:
	int currentScene;
	int newScene;
	bool sceneLoaded;
};