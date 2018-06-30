#pragma once
#include <string>
#include "tinyxml2.h"
#include "ECS.hpp"
#include "Vector2D.hpp"

using namespace tinyxml2;

class SpellHandler {
public:
	float sSpeed;
	float deltaButton;
	float angleX, angleY;

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
	XMLElement* sElementSprite;


	float deltaMultiplier;

	int sID;
	int lastSpellID = 0;

	int mouseX, mouseY;
	int playerX, playerY;

	const char * sPath = "-";

	std::string sName;
	const char * sSprite;
	int sType;

	float sDeltaTime;

	float sSpeedMin, sSpeedMax;
	float sSize, sSizeMin, sSizeMax;
	int sDistance, sDistanceMin, sDistanceMax;
	int sDmg, sDmgMin, sDmgMax;
	int sMana, sManaMin, sManaMax;

	int sWidth;
	int sHeight;
	int sFrameDelay;
	float minFrameDelay, maxFrameDelay;
	int sFrameCount, sFrameCountLoad, sFrameCountDelete;
	float sDeleteTime;

	int sIndex, sIndexLoad, sIndexDelete;
	int sFrameDelayLoad, sFrameDelayDelete;

};