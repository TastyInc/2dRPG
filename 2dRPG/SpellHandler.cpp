#include "SpellHandler.hpp"
#include "ECS.hpp"
#include "Components.hpp"
#include "game.hpp"
#include <math.h>

#define PI 3.1415926535

extern Manager manager;

SpellHandler::SpellHandler() {

}

SpellHandler::~SpellHandler() {

}

void SpellHandler::updatePlayerPos(int pX, int pY) {
	playerX = pX + 32;
	playerY = pY;
}

void SpellHandler::LoadSpell(int spellID, float deltaBtn, int mX, int mY) {
	deltaButton = deltaBtn;
	mouseX = mX + Game::camera->getCamera().x;
	mouseY = mY + Game::camera->getCamera().y;

	double angleDist = sqrt(pow(mouseX - playerX, 2.0f) + pow(mouseY - playerY, 2.0f));

	angleX = (mouseX - playerX) / angleDist;
	angleY = (mouseY - playerY) / angleDist;

	LoadSpell(spellID);
}

bool SpellHandler::LoadSpell(int spellID) {
	
	if(lastSpellID != spellID){
		eResult = spellFile.LoadFile("resources/spells.xml");
		if (eResult != XML_SUCCESS) {
			std::cout << "Could not load SpellFile" << std::endl;
			return false;
		}

		XMLNode* root = spellFile.FirstChildElement("spells");
		if (root == nullptr) {
			return false;
		}

		spell = root->FirstChildElement("spell");
		spell->QueryIntAttribute("id", &sID);

		//loop through Spells
		while (sID != spellID) {
			spell = spell->NextSiblingElement("spell");
			spell->QueryIntAttribute("id", &sID);
		}

		if (spell == NULL) {
			std::cout << "Could not find Spell... :(" << std::endl;
			return false;
		}

		spell->QueryIntAttribute("type", &sType);
		const char* str = "";
		spell->QueryStringAttribute("name", &str);
		sName = str;
		spell->QueryStringAttribute("sprite", &sSprite);

		//Switch bitte no SEEEEEEEEEEEEEHR starch abendere...
		switch (sType) {
		case 1:
			sElement = getElementByName(*spell, "delta");
			if (sElement != NULL) sElement->QueryFloatAttribute("time", &sDeltaTime);

			sElement = getElementByName(*spell, "transform");
			if (sElement != NULL) {
				sElement->QueryFloatAttribute("speedMin", &sSpeedMin);
				sSpeedMax = sSpeedMin;
				sElement->QueryFloatAttribute("speedMax", &sSpeedMax);
				sElement->QueryFloatAttribute("sizeMin", &sSizeMin);
				sSizeMax = sSizeMin;
				sElement->QueryFloatAttribute("sizeMax", &sSizeMax);
				sElement->QueryIntAttribute("distMin", &sDistanceMin);
				sDistanceMax = sDistanceMin;
				sElement->QueryIntAttribute("distMax", &sDistanceMax);
			}

			sElement = getElementByName(*spell, "damage");
			if (sElement != NULL) {
				sElement->QueryIntAttribute("dmgMin", &sDmgMin);
				sDmgMax = sDmgMin;
				sElement->QueryIntAttribute("dmgMax", &sDmgMax);
			}

			sElement = getElementByName(*spell, "mana");
			if (sElement != NULL) {
				sElement->QueryIntAttribute("manaMin", &sDmgMin);
				sDmgMax = sDmgMin;
				sElement->QueryIntAttribute("manaMax", &sDmgMax);
			}

			sElement = getElementByName(*spell, "sprite");
			if (sElement != NULL) {
				sElement->QueryIntAttribute("width", &sWidth);
				sElement->QueryIntAttribute("height", &sHeight);
				sElement->QueryFloatAttribute("minFrameDelay", &minFrameDelay);
				maxFrameDelay = minFrameDelay;
				sElement->QueryFloatAttribute("maxFrameDelay", &maxFrameDelay);
				sElement->QueryIntAttribute("frameCount", &sFrameCount);
				sElement->QueryIntAttribute("index", &sIndex);

				sElementSprite = getElementByName(*sElement, "load");
				if (sElementSprite != NULL) {
					sElementSprite->QueryIntAttribute("frameDelay", &sFrameDelayLoad);
					sElementSprite->QueryIntAttribute("index", &sIndexLoad);
					sElementSprite->QueryIntAttribute("frameCountLoad", &sFrameCountLoad);
				}

				sElementSprite = getElementByName(*sElement, "delete");
				if (sElementSprite != NULL) {
					sElementSprite->QueryIntAttribute("frameDelay", &sFrameDelayDelete);
					sElementSprite->QueryIntAttribute("index", &sIndexDelete);
					sElementSprite->QueryIntAttribute("frameCountDelete", &sFrameCountDelete);
				}
			}
			break;
		default:
			break;
		}

		lastSpellID = sID;
	}

	if (deltaButton > sDeltaTime) {
		deltaMultiplier = sDeltaTime;
	} else {
		deltaMultiplier = deltaButton / sDeltaTime;
	}

	sSpeed = minMax(sSpeedMin, sSpeedMax);
	sSize = minMax(float(sSizeMin), float(sSizeMax));
	sDistance = minMax(sDistanceMin, sDistanceMax);
	sDmg = minMax(sDmgMin, sDmgMax);
	sMana = minMax(sManaMin, sManaMax);
	sFrameDelay = int(minMax(minFrameDelay, maxFrameDelay));

	createSpell();

	return true;
}

void SpellHandler::createSpell() {

	auto& spell(manager.addEntity());

	Game::assets->AddTexture(sName, sSprite);

	spell.addComponent<SpellComponent>(sDmg, sMana);
	spell.addComponent<TransformComponent>(playerX, playerY, sWidth, sHeight, sSize);
	spell.addComponent<SpriteComponent>(sName, sFrameCount, sFrameDelay, sIndex, "spell");
	spell.getComponent<SpriteComponent>().addAnimation(sFrameCountLoad, sFrameDelayLoad, sIndexLoad, "spell_load");
	spell.getComponent<SpriteComponent>().addAnimation(sFrameCountDelete, sFrameDelayDelete, sIndexDelete, "spell_destroy");
	spell.addComponent<ProjectileComponent>(sDistance, sSpeed, Vector2D(sSpeed * angleX, sSpeed * angleY));
	spell.addComponent<ColliderComponent>("spell");
	spell.addGroup(Game::groupSpells);

}

int SpellHandler::minMax(int min, int max) {
	if (min != max) {
		int diff = max - min;

		return int(min + diff * deltaMultiplier);
	}

	return min;
}

float SpellHandler::minMax(float min, float max) {
	if (min != max) {
		float diff = max - min;

		return min + diff * deltaMultiplier;
	}

	return min;
}

XMLElement* SpellHandler::getElementByName(XMLElement &ele, std::string const &elem_name) {
	XMLElement* elem = ele.FirstChildElement();
	while (elem){
		if (!std::string(elem->Value()).compare(elem_name)){
			return elem;
		}
		elem = elem->NextSiblingElement();
	}

	return NULL;
}