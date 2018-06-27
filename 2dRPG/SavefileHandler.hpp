#pragma once
#include <fstream>
#include <string>
#include <sstream>  
#include "Vector2D.hpp"
#include <vector>

class SavefileHandler {
public:
	int playerMaxHealth;
	int playerCurrHealth;
	int playerStamina;
	int playerLevel;
	int mapTheme;
	int mapScreen;
	Vector2D playerPos;
	int items[100]; //je nachdem angeri zahl. 100 grad chli viel

	SavefileHandler() {}

	~SavefileHandler() {}

	//de no apasse mit de argument
	int saveGame(Vector2D pos, int mtID, int mcID) {
		std::fstream savefile;
		savefile.open("resources/savefile.sv");
		if (!savefile.is_open()) {
			return 0;
		}

		mapTheme = mtID;
		mapScreen = mcID;

		std::string line;

		line = "savefile ";
		line += "playerpos ";
		line += std::to_string(mapTheme) + " ";
		line += std::to_string(mapScreen) + " ";
		line += std::to_string(int(pos.x)) + " "; 
		line += std::to_string(int(pos.y)) + " ";
		line += "playerstats ";

		line += "100 100 100 1 playeritems 1 2 3 4 5 6";
		//line = encryptDecrypt(line);

		savefile << line;

		savefile.close();

		return 1;
	}

	int loadGame() {
		std::fstream savefile;
		savefile.open("resources/savefile.sv");
		if (!savefile.is_open()){
			return 0;
		}

		std::string line;
		std::getline(savefile, line);
		//line = encryptDecrypt(line);
		std::stringstream ssLine(line);
		std::string keyWord;

		ssLine >> keyWord;
		if (keyWord == "savefile"){
			ssLine >> keyWord;
			if (keyWord == "playerpos") {

				ssLine >> keyWord;
				mapTheme = atoi(keyWord.c_str());
				ssLine >> keyWord;
				mapScreen = atoi(keyWord.c_str());
				ssLine >> keyWord;
				playerPos.x = float(atof(keyWord.c_str()));
				ssLine >> keyWord;
				playerPos.y = float(atof(keyWord.c_str()));
				ssLine >> keyWord;

				if (keyWord == "playerstats") {
					
					ssLine >> keyWord;
					playerMaxHealth = atoi(keyWord.c_str());
					ssLine >> keyWord;
					playerCurrHealth = atoi(keyWord.c_str());
					ssLine >> keyWord;
					playerStamina = atoi(keyWord.c_str());
					ssLine >> keyWord;
					playerLevel = atoi(keyWord.c_str());
					ssLine >> keyWord;

					if (keyWord == "playeritems") {
						int itemNum = 0;
						while (ssLine >> keyWord) {
							items[itemNum] = atoi(keyWord.c_str());
							itemNum++;
						}

						savefile.close();
						return 1;
					}
				}
			}
		}
		
		savefile.close();
		return 0;
	}

private:

	std::string encryptDecrypt(std::string toEncrypt) {
		char key[3] = { 'L', 'A', 'S' }; //keys for added security
		std::string output = toEncrypt;

		for (int i = 0; i < toEncrypt.size(); i++)
			output[i] = toEncrypt[i] ^ key[i % (sizeof(key) / sizeof(char))];

		return output;
	}
};