#include "Coin.hpp"

Coin::Coin(float x, float y, float w, float h, char type) 
    : MovingObject(x, y, w, h, '$') {
    vertSpeed = -0.5f;
}
void Coin::updatePhysics(Map& map, GameObject** allObjects, int count) {
    vertSpeed += 0.05f;
    float nextY = y + vertSpeed;
    
    float nextX = x + horizonSpeed;

    bool onGround = false;
    bool wallAhead = false;
    bool floorAhead = false;

	for (int i = 0; i < count; i++) {
		if (allObjects[i] == nullptr || allObjects[i] == this) continue;
		if (allObjects[i]->getType() == '@') continue;

        GameObject futureEnemyY(x, nextY, width, height, this->cType);
        if (futureEnemyY.isCollision(*allObjects[i])) {
            if (vertSpeed > 0) {
                onGround = true;
                y = allObjects[i]->getY() - height;
                vertSpeed = 0;
            }
        }

        GameObject futureEnemyX(nextX, y, width, height, this->cType);
        if (futureEnemyX.isCollision(*allObjects[i])) {
            wallAhead = true;
        }

        GameObject futureFloorCheck(nextX, y + height + 0.1f, width, height, this->cType);
        if (futureFloorCheck.isCollision(*allObjects[i])) {
            floorAhead = true;
        }
    }

    if (wallAhead) {
        horizonSpeed *= -1;
    }

    x += horizonSpeed;
    if (!onGround) {
        y = nextY;
    }
}