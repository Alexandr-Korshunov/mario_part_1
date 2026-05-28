#pragma once
#include "MovingObject.hpp"

class Enemy : public MovingObject {
	public:
		Enemy(float xPos, float yPos, float oWidth, float oHeight, char type);
		
		void updatePhysics(Map& map, GameObject** allObjects, int count) override;
		void update() override;
};