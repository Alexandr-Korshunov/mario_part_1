#include "Enemy.hpp"

Enemy::Enemy(
	float xPos, float yPos,
	float oWidth, float oHeight,
	char type)
	: MovingObject(xPos, yPos, oWidth, oHeight, type){
		vertSpeed = 0;
		horizonSpeed = 0.2;
}

void Enemy::updatePhysics(Map& map, GameObject** allObjects, int count) {
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

    if (wallAhead || !floorAhead) {
        horizonSpeed *= -1;
    }

    x += horizonSpeed;
    if (!onGround) {
        y = nextY;
    }
}

void Enemy::update() {
}
	