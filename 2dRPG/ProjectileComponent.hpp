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

			if (!entity->hasGroup(Game::groupSpells)) {
				entity->destroy();
			} else {
				pTimer->Update();
				entity->getComponent<SpriteComponent>().Play("spell_destroy");
				entity->getComponent<SpriteComponent>().lastAnimation();
			}
		} else {
			pTimer->Reset();
		}
	}

	void destroyProjectile() {
		
	}

private:
	Timer* pTimer;

	TransformComponent* transform;
	
	int range = 0;
	int speed = 0;
	int distance = 0;

	Vector2D velocity;

};