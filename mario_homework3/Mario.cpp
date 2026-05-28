#include "Mario.hpp"
#include <iostream>
Mario::Mario(
	float xPos, float yPos,
	float oWidth, float oHeight,
	char type)
	: MovingObject(xPos, yPos, oWidth, oHeight, type){
		isFly = true;
		hitBlock = nullptr;
}

bool Mario::getIsFly() const { return isFly; }

void Mario::setIsFly( bool newIsFly ){
	isFly = newIsFly;
}

void Mario::updatePhysics(Map& map, GameObject** allObjects, int count) {
    vertSpeed += 0.05f;

    float nextY = y + vertSpeed;
    GameObject futureMario(x, nextY, width, height, this->cType);

    bool collision = false;
    GameObject* hitObject = nullptr;

    for (int i = 0; i < count; i++) {
        if (allObjects[i] == nullptr || allObjects[i] == this) continue;
        if (allObjects[i]->getType() == '$') continue;
        if (allObjects[i]->getType() == 'o') continue;
		if (allObjects[i]->getType() == '+') continue;

        if (futureMario.isCollision(*allObjects[i])) {
            collision = true;
            hitObject = allObjects[i];
            break;
        }
    }

    if (collision) {
        if (vertSpeed > 0) {
            y = hitObject->getY() - height;
            vertSpeed = 0;
            isFly = false;
        } else if (vertSpeed < 0) {
            y = hitObject->getY() + hitObject->getHeight();
            vertSpeed = 0;
            hitBlock = hitObject;
        }
    } else {
        y = nextY;
        isFly = true;
    }
}