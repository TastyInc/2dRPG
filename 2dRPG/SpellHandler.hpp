#pragma once
#include <string>
#include "tinyxml2.h"
#include "Vector2D.hpp"

using namespace tinyxml2;

class SpellHandler {
public:
	SpellHandler();
	~SpellHandler();

	void LoadSpell(int spellID, float deltaBtn, int mX, int mY);
	bool LoadSpell(int spellID);
	void updatePlayerPos(int pX, int pY);

private:

	XMLElement* getElementByName(XMLElement &ele, std::string const & elem_name);
	bool spellLoaded = false;
	int minMax(int min, int max);
	float minMax(float min, float max);

	void createSpell();

	XMLDocument spellFile;
	XMLError eResult;
	XMLElement* spell;
	XMLElement* sElement;

	float deltaButton;
	float deltaMultiplier;
	double angleX, angleY;

	int sID;
	int lastSpellID = 0;

	int mouseX, mouseY;
	int playerX, playerY;

	const char * sPath = "-";

	std::string sName;
	const char * sSprite;
	int sType;

	float sDeltaTime;

	float sSpeed, sSpeedMin, sSpeedMax;
	float sSize, sSizeMin, sSizeMax;
	int sDistance, sDistanceMin, sDistanceMax;
	int sDmg, sDmgMin, sDmgMax;

	int sWidth;
	int sHeight;
	int sFrameDelay;
	float minFrameDelay, maxFrameDelay;
	int sAnimCount;

};