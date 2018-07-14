#pragma once

struct Animation {
	int index;
	int frames;
	int speed;
	double angle;

	Animation() {}

	Animation(int i, int f, int s) {
		index = i;
		frames = f;
		speed = s;
		angle = 0;
	}

	Animation(int i, int f, int s, double a) {
		index = i;
		frames = f;
		speed = s;
		angle = a;
	}
};