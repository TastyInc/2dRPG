#pragma once
#include "Vector2D.hpp"
#include "SDL.h"

class CameraHandler {
public:
	SDL_Rect camera;

	CameraHandler(int x, int y, int h, int w) {
		camera.h = h;
		camera.w = w;
		camera.x = x;
		camera.y = y;
	}

	~CameraHandler() {
	}

	void cameraShake(int x, int y) {
		
	}

	void updateCameraPos(int x, int y) {
		camera.x = x;
		camera.y = y;

		if (camera.x < 0) {
			camera.x = 0;
		}
		if (camera.y < 0) {
			camera.y = 0;
		}
		if (camera.x > camera.w) {
			camera.x = camera.w;
		}
		if (camera.y > camera.h) {
			camera.y = camera.h;
		}
	}

	void updateCameraSize(Vector2D size, int windowX, int windowY) {
		camera.w = int(size.x) - windowX;
		camera.h = int(size.y) - windowY;
	}

	SDL_Rect getCamera() {
		return camera;
	}
};