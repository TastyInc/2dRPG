#pragma once
#include "ECS.hpp"
#include "Components.hpp"
#include "Vector2D.hpp"

class ProjectileComponent : public Component {
public:
	ProjectileComponent(int rng, int sp, Vector2D vel) : range(rng), speed(sp), velocity(vel) {
	}

	~ProjectileComponent() {};

	void init() override{
		transform = &entity->getComponent<TransformComponent>();
		transform->velocity = velocity;
	}

	void update() override {
		distance += speed;

		if (distance > range) {
			entity->destroy();
			//entity->getComponent<SpellComponent>().destroyParticle;
		} else if (	transform->position.x > Game::camera->camera.x + Game::camera->camera.w ||
					transform->position.x < Game::camera->camera.x ||
					transform->position.y > Game::camera->camera.y + Game::camera->camera.h ||
					transform->position.y < Game::camera->camera.y) {
			entity->destroy();
		}
	}

private:
	TransformComponent* transform;
	
	int range = 0;
	int speed = 0;
	int distance = 0;

	Vector2D velocity;

};