#pragma once
#include "SDL.h"

struct HudElement {
	SDL_Rect pos;

	int index;
	int frames;
	int speed;
	double angle;

	HudElement() {}

	HudElement(int i, int f, int s) {
		index = i;
		frames = f;
		speed = s;
	}

	HudElement(int i, int f, int s, double a) {
		index = i;
		frames = f;
		speed = s;
		angle = a;
	}
};