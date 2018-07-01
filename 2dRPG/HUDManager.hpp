#pragma once
#include "tinyxml2.h"
#include <iostream>

using namespace tinyxml2;

class HUDManager {
public:
	int id;
	const char* name;
	const char* sprite;
	const char* type;
	float posx;
	float posy;
	int sizex;
	int sizey;

	int currentID = 0;

	HUDManager() {

	};

	bool loadHUD() {
		eResult = hudFile.LoadFile("resources/hud.xml");
		if (eResult != XML_SUCCESS) {
			std::cout << "Could not load HUDFile" << std::endl;
			return false;
		}

		root = hudFile.FirstChildElement("hud");
		if (root == nullptr) {
			std::cout << "Could not load HUDFile" << std::endl;
			return false;
		}

		root->QueryIntAttribute("elementCount", &elementCount);

		return true;
	}

	bool getHudElement() {

		currentID++;

		if (currentID <= elementCount) {
			for (XMLElement* e = root->FirstChildElement("hudElement"); e != NULL; e = e->NextSiblingElement("hudElement")) {

				e->QueryIntAttribute("id", &id);
				if (id == currentID) {
					e->QueryFloatAttribute("posx", &posx);
					e->QueryFloatAttribute("posy", &posy);
					e->QueryIntAttribute("sizex", &sizex);
					e->QueryIntAttribute("sizey", &sizey);
					e->QueryStringAttribute("name", &name);
					e->QueryStringAttribute("sprite", &sprite);
					if (e->QueryStringAttribute("type", &type) != 0) {
						type = NULL;
					}
					returnValue = true;
					break;
				}
				else {
					returnValue = false;
				}
			}
		} else {
			return false;
		}

		return returnValue;
	}

private:
	XMLDocument hudFile;
	XMLError eResult;
	XMLElement* hudElement;
	XMLElement* root;

	bool returnValue = false;
	int elementCount;
};