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

		pTimer = Timer::Instance();
	}

	void update() override {
		distance += speed;

		if (distance > range) {

			if (entity->hasGroup(Game::groupSpells)) {
				entity->getComponent<SpriteComponent>().Play("destroy");
				if (destroyTime > 30) {

					entity->destroy();
					destroyTime = 0;
				}
				destroyTime++;
			} else {
				entity->destroy();
			}
		} 
	}

private:
	Timer* pTimer;

	SpellComponent* spellC;
	TransformComponent* transform;
	
	int range = 0;
	int speed = 0;
	int distance = 0;

	int destroyTime = 0;

	Vector2D velocity;

};