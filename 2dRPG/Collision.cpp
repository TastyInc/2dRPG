#include "Collision.hpp"
#include "ColliderComponent.hpp"

int Collision::AABBxy(const SDL_Rect& recA, const SDL_Rect& recB) {

	if (
		recA.x + recA.w >= recB.x &&
		recB.x + recB.w >= recA.x &&
		recA.y + recA.h >= recB.y &&
		recB.y + recB.h >= recA.y
		){

		double angle = atan2((recB.y + recB.h / 2) - (recA.y + recA.h / 2), (recB.x + recB.w / 2) - (recA.x + recA.w / 2)) * 180 / s_PI;

		if (angle == 45.0f) {
			return 5;
		} else if (angle == 135.0f) {
			return 5;
		} else if (angle == -45.0f) {
			return 5;
		} else if (angle == -135.0f) {
			return 5;
		}

		// <-P
		if (angle > -45 && angle <= 0 || angle > 0 && angle <= 45) {
			return 1;
		}
		// /|\ p
		if (angle > 45 && angle <= 135) {
			return 2;
		}
		// P ->
		if (angle > 135 && angle <= 180 || angle >= -180 && angle <= -135) {
			return 3;
		}
		// \|/ P
		if (angle > -135 && angle <= -45) {
			return 4;
		}

		return 1;
	}

	return 0;
}

bool Collision::AABB(const SDL_Rect& recA, const SDL_Rect& recB) {
	if (
		recA.x + recA.w >= recB.x &&
		recB.x + recB.w >= recA.x &&
		recA.y + recA.h >= recB.y &&
		recB.y + recB.h >= recA.y
		)
	{
		return true;
	}

	return false;
}

int Collision::AABBxy(const ColliderComponent& colA, const ColliderComponent& colB) {
	switch (AABBxy(colA.collider, colB.collider)){
		case 0:
			return 0;
			break;
		case 1:
			return 1;
			break;
		case 2:
			return 2;
			break;
		case 3: 
			return 3;
			break;
		case 4:
			return 4;
			break;
		default:
			return 0;
			break;
	}
}

bool Collision::AABB(const ColliderComponent& colA, const ColliderComponent& colB) {
	if (AABB(colA.collider, colB.collider)) {
		return true;
	}
	else {
		return false;
	}

}