#pragma once
#include "GameObject.hpp"

class Map {
private:
    char** grid;
    int width;
    int height;

    bool isPosValid(int x, int y) const;

public:
    Map(int mapWidth, int mapHeight);
    ~Map();

    int getWidth() const;
    int getHeight() const;

	char getChar(int x, int y) const;

    void clear();
    void setChar(int x, int y, char symbol);
    void display();

    void scrollMap(float dx, GameObject* player, GameObject** objects, int objectsCount);
};