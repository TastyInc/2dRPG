#pragma once

#include "game.hpp"
#include "ECS.hpp"
#include "Components.hpp"
#include "Vector2D.hpp"

class TransformComponent : public Component {
private:
	int xpos = 0;
	int ypos = 0;

public:
	Vector2D position;
	Vector2D velocity;

	int height = 32;
	int width = 32;
	float scale = 1.0f;

	int speed = 3;

	//Nice Overloads hie 
	TransformComponent() {
		position.Zero();
	}

	//init pos f�r player. de speter no endere f�r enemies etc
	TransformComponent(float sc) {
		position.x = 900;
		position.y = 700;
		scale = sc;
	}

	TransformComponent(float x, float y) {
		position.x = x;
		position.y = y;
	}

	//posX, posY, height, width, scale
	TransformComponent(float x, float y, int w, int h, float sc) {
		position.x = x;
		position.y = y;
		height = h;
		width = w;
		scale = sc;
	}


	void init() override {
		velocity.Zero();
	}

	void update() override {
		if (velocity.x != 0) {
		
		} else {
			//sprite->Play("Idle");
		}


		position.x += velocity.x * speed;
		position.y += velocity.y * speed;
	}
};