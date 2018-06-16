#pragma once
#include "tinyxml2.h"

using namespace tinyxml2;

class MenuHandler {
public:
	int eleCount;
	SDL_Rect menuRect[6];
	std::string menuText[6];

	MenuHandler() {}
	~MenuHandler() {}

	/*void draw() {
		for (int i = 0; i < eleCount; i++){
			
		}
	}*/

	void LoadMenu(int menu) {
		eleCount = 0;

		eResult = menuFile.LoadFile("resources/menus.xml");
		if (eResult != XML_SUCCESS) {
		std::cout << "Could not load MenuFile" << std::endl;
		}

		XMLNode* root = menuFile.FirstChildElement("menulist");
		if (root == nullptr) {
		//ERROR
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
			menuRect[eleCount] = { 100, eleCount * 80 + 300, 500, 50 };

			eleCount++;

		}
	}

	void buttonPressed() {

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
		currentScene = 0;
	}

	~SceneManager() {}

	void setScene(int scene) {
		currentScene = scene;
		if (currentScene != 0) {
			menus->LoadMenu(currentScene);
		}

	}

	int getScene(std::string id) {

		//return scene;

	}

	int getSceneElementCount() {
		return menus->eleCount;
	}

	int getCurrentScene() {
		return currentScene;
	}

	/*void draw() {
		menus->draw();
	}*/

private:
	int currentScene;
	bool sceneLoaded;
};