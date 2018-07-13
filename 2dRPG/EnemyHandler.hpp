#pragma once
#include "ECS.hpp"

extern Manager manager;

class EnemyHandler{
public:
	EnemyHandler();

	~EnemyHandler();

	void loadEnemy();

	void createEnemy();

	void destroyEnemy();


private:


};