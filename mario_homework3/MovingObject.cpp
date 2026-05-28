#include "MovingObject.hpp"

MovingObject::MovingObject(
	float xPos, float yPos,
	float oWidth, float oHeight,
	char type)
	: GameObject(xPos, yPos, oWidth, oHeight, type){
		vertSpeed = 0;
		horizonSpeed = 0.2;
}

float MovingObject::getVertSpeed() const { return vertSpeed; }
float MovingObject::getHorizonSpeed() const { return horizonSpeed; }

void MovingObject::setVertSpeed( float newVertSpeed ){
	vertSpeed = newVertSpeed;
}
void MovingObject::setHorizonSpeed( float newHorizonSpeed ){
	horizonSpeed = newHorizonSpeed;
}

void MovingObject::updatePhysics(Map& map, GameObject** allObjects, int count) {
}

void MovingObject::update() {
}