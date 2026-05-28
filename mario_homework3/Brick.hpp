#pragma once
#include "GameObject.hpp"

class Brick : public GameObject {
public:
    Brick(float xPos, float yPos, float oWidth, float oHeight, char type);
};