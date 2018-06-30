#pragma once
#include "tinyxml2.h"
#include <iostream>

using namespace tinyxml2;

class HUDManager {
public:
	int elemCount = 9;
	int id[9];
	const char* name[9];
	const char* sprite[9];
	float posx[9];
	float posy[9];
	int sizex[9];
	int sizey[9];


	HUDManager() {};

	~HUDManager() {
	};

	int loadHUD() {
		eResult = hudFile.LoadFile("resources/hud.xml");
		if (eResult != XML_SUCCESS) {
			std::cout << "Could not load HUDFile" << std::endl;
			return 0;
		}

		XMLNode* root = hudFile.FirstChildElement("hud");
		if (root == nullptr) {
			std::cout << "Could not load HUDFile" << std::endl;
			return 0;
		}

		int count = 0;
		for (XMLElement* e = root->FirstChildElement("hudElement"); e != NULL; e = e->NextSiblingElement("hudElement")){

			e->QueryIntAttribute("id", &id[count]);
			e->QueryFloatAttribute("posx", &posx[count]);
			e->QueryFloatAttribute("posy", &posy[count]);
			e->QueryIntAttribute("sizex", &sizex[count]);
			e->QueryIntAttribute("sizey", &sizey[count]);
			e->QueryStringAttribute("name", &name[count]);
			e->QueryStringAttribute("sprite", &sprite[count]);

			count++;
		}	
		return 1;
	}

private:
	XMLDocument hudFile;
	XMLError eResult;
	XMLElement* hudElement;


};