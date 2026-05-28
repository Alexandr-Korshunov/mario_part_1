#include "Map.hpp"
#include <iostream>
#include <windows.h>
#include <cstdio>
#include <cmath>

Map::Map(int mapWidth, int mapHeight) {
    width = mapWidth;
    height = mapHeight;

    grid = new char*[height];
    for (int i = 0; i < height; i++) {
        grid[i] = new char[width + 1];
    }
    clear();
}

Map::~Map() {
    for (int i = 0; i < height; i++) {
        delete[] grid[i];
    }
    delete[] grid;
}

int Map::getWidth() const { return width; }
int Map::getHeight() const { return height; }

bool Map::isPosValid(int x, int y) const {
    return (x >= 0 && x < width && y >= 0 && y < height);
}

char Map::getChar(int x, int y) const {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        return grid[y][x];
    }
    return ' ';
}

void Map::clear() {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            grid[i][j] = ' ';
        }
        grid[i][width] = '\0';
    }
}

void Map::setChar(int x, int y, char symbol) {
    if (isPosValid(x, y)) {
        grid[y][x] = symbol;
    }
}

void Map::display() {
    COORD coord = { 0, 0 };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

    for (int i = 0; i < height; i++) {
        printf("%s\n", grid[i]);
    }
}

void Map::scrollMap(float dx, GameObject* player, GameObject** objects, int objectsCount) {
    if (player == nullptr) return;

    player->setX(player->getX() - dx);

    bool collision = false;
	for (int i = 0; i < objectsCount; i++) {
		if (objects[i] != nullptr && player->isCollision(*objects[i])) {
			if (objects[i]->getType() == 'o') continue;
			if (objects[i]->getType() == '$') continue;
			if (objects[i]->getType() == '+') continue;
			collision = true;
			break;
		}
	}

    player->setX(player->getX() + dx);

    if (!collision) {
        for (int i = 0; i < objectsCount; i++) {
            if (objects[i] != nullptr) {
                objects[i]->setX(objects[i]->getX() + dx);
            }
        }
    }
}