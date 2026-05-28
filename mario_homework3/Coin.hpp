#pragma once
#include "MovingObject.hpp"

class Coin : public MovingObject {
public:
    Coin(float x, float y, float w, float h, char type);
    void updatePhysics(Map& map, GameObject** allObjects, int count) override;
};