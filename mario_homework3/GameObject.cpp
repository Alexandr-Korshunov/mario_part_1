#include "GameObject.hpp"
#include <cmath>

GameObject::GameObject(
	float xPos, float yPos,
	float oWidth, float oHeight,
	char type) {
    x = xPos;
    y = yPos;
    width = oWidth;
    height = oHeight;
    cType = type;
}

GameObject::~GameObject() {}

float GameObject::getX() const { return x; }
float GameObject::getY() const { return y; }
float GameObject::getWidth() const { return width; }
float GameObject::getHeight() const { return height; }
char GameObject::getType() const { return cType; }

void GameObject::setX(float newX) { x = newX; }
void GameObject::setY(float newY) { y = newY; }
void GameObject::setType(char newType) { cType = newType; }

bool GameObject::isCollision(const GameObject& other) const {
    return (x + width > other.getX()) && (x < other.getX() + other.getWidth()) &&
           (y + height > other.getY()) && (y < other.getY() + other.getHeight());
}