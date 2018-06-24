#pragma once
#include "tinyxml2.h"
#include <iostream>
#include "Vector2D.hpp"

using namespace tinyxml2;

class HUDManager {
public:
	int elemCount = 6;
	int id[6];
	const char* name[6];
	const char* sprite[6];
	float posx[6];
	float posy[6];
	int sizex[6];
	int sizey[6];


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
	}

private:
	XMLDocument hudFile;
	XMLError eResult;
	XMLElement* hudElement;


};