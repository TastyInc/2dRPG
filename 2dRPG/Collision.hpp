#pragma once
#include "SDL.h"

class ColliderComponent;

class Collision {
private:
	#define s_PI 3.14159265358979323846  /*PI*/

public:
	//Access Align Bounding Box | AABBxy für spezifische collision x oder y
	static int AABBxy(const SDL_Rect& rectA, const SDL_Rect& rectB);
	static int AABBxy(const ColliderComponent& colA, const ColliderComponent& colB);

	static int AABBx(const SDL_Rect& rectA, const SDL_Rect& rectB);
	static int AABBy(const SDL_Rect& rectA, const SDL_Rect& rectB);

	static bool AABB(const SDL_Rect& rectA, const SDL_Rect& rectB);
	static bool AABB(const ColliderComponent& colA, const ColliderComponent& colB);
};