#pragma once
#include <iostream>
#include <fstream>

class SavefileHandler {
public:
	std::ofstream safefile;

	SavefileHandler() {
		safefile.open("savefile.sv");
	}

	~SavefileHandler() {
	
	}

	int loadSafefile() {
		if (!safefile.is_open()){
			return 0;
		}

		return 1;
	}

	std::string encryptDecrypt(std::string toEncrypt) {
		char key[3] = { 'N', 'I', 'G' }; //keys for added security
		std::string output = toEncrypt;

		for (int i = 0; i < toEncrypt.size(); i++)
			output[i] = toEncrypt[i] ^ key[i % (sizeof(key) / sizeof(char))];

		return output;
	}

private:
	//viel error messages gib witer "health" oder so nere lueg im .save file öb vore "health" steit u we nid de return e error.


};