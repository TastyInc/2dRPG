#pragma once
#include "ECS.hpp"
#include "AssetManager.hpp"
#include "TextureManager.hpp"
#include "game.hpp" 

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

class UILable : public Component {
public:
	UILable(int xpos, int ypos, std::string text, std::string font, SDL_Color& color) :
		labelText(text), LabelFont(font), textColor(color) 
	{
		position.x = xpos;
		position.y = ypos;
	
		SetLableText(labelText, LabelFont);
	}

	~UILable() {}

	void SetLableText(std::string text, std::string font) {
		SDL_Surface* surf = TTF_RenderText_Blended(Game::assets->GetFont(font), text.c_str(), textColor);
		labelTexture = SDL_CreateTextureFromSurface(Game::renderer, surf);
		SDL_FreeSurface(surf);

		SDL_QueryTexture(labelTexture, nullptr, nullptr, &position.w, &position.h);
	}

	void draw() override {
		SDL_RenderCopy(Game::renderer, labelTexture, nullptr, &position);
	}

private:
	SDL_Rect position;
	std::string labelText;
	std::string LabelFont;
	SDL_Color textColor;
	SDL_Texture* labelTexture;

};