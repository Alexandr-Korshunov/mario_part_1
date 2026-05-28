#include "Level.hpp"
#include "Brick.hpp"
#include "Enemy.hpp"
#include "Coin.hpp"
#include <windows.h>
#include <cmath>
#include <iostream>
#include <cstdio>
#include <cstring>

Level::Level(int width, int height) 
    : levelMap(width, height), player(nullptr), gameObjects(nullptr), 
      capacity(10), count(0), currentLevel(1), maxLevel(3), score(0)
{
    player = new Mario(39.0f, 10.0f, 3.0f, 3.0f, '@');
    gameObjects = new GameObject*[capacity];
    for (int i = 0; i < capacity; i++) gameObjects[i] = nullptr;
    loadLevel(currentLevel);
}

Level::~Level() {
    delete player;
    if (gameObjects != nullptr) {
        for (int i = 0; i < count; i++) {
            if (gameObjects[i] != nullptr) {
                delete gameObjects[i];
            }
        }
        delete[] gameObjects;
    }
}

void Level::loadLevel(int lvl) {
    if (lvl == 1) {
        addGameObject(new Brick(20.0f, 20.0f, 40.0f, 5.0f, '#'));
        addGameObject(new Brick(30.0f, 10.0f, 5.0f, 3.0f, '?'));
        addGameObject(new Brick(50.0f, 10.0f, 5.0f, 3.0f, '?'));
        addGameObject(new Brick(60.0f, 15.0f, 40.0f, 10.0f, '#'));
        addGameObject(new Brick(60.0f, 5.0f, 10.0f, 3.0f, '-'));
        addGameObject(new Brick(70.0f, 5.0f, 5.0f, 3.0f, '?'));
        addGameObject(new Brick(75.0f, 5.0f, 5.0f, 3.0f, '-'));
        addGameObject(new Brick(80.0f, 5.0f, 5.0f, 3.0f, '?'));
        addGameObject(new Brick(85.0f, 5.0f, 10.0f, 3.0f, '-'));
        addGameObject(new Brick(100.0f, 20.0f, 20.0f, 5.0f, '#'));
        addGameObject(new Brick(120.0f, 15.0f, 10.0f, 10.0f, '#'));
        addGameObject(new Brick(150.0f, 20.0f, 40.0f, 5.0f, '#'));
        addGameObject(new Brick(210.0f, 15.0f, 10.0f, 10.0f, '+'));
        addGameObject(new Enemy(25.0f, 10.0f, 3.0f, 2.0f, 'o'));
        addGameObject(new Enemy(80.0f, 10.0f, 3.0f, 2.0f, 'o'));
    }
    else if (lvl == 2) {
        addGameObject(new Brick(20.0f, 20.0f, 40.0f, 5.0f, '#'));
        addGameObject(new Brick(60.0f, 15.0f, 10.0f, 10.0f, '#'));
        addGameObject(new Brick(80.0f, 20.0f, 20.0f, 5.0f, '#'));
        addGameObject(new Brick(120.0f, 15.0f, 10.0f, 10.0f, '#'));
        addGameObject(new Brick(150.0f, 20.0f, 40.0f, 5.0f, '#'));
        addGameObject(new Brick(210.0f, 15.0f, 10.0f, 10.0f, '+'));
        addGameObject(new Enemy(25.0f, 10.0f, 3.0f, 2.0f, 'o'));
        addGameObject(new Enemy(80.0f, 10.0f, 3.0f, 2.0f, 'o'));
        addGameObject(new Enemy(65.0f, 10.0f, 3.0f, 2.0f, 'o'));
        addGameObject(new Enemy(120.0f, 10.0f, 3.0f, 2.0f, 'o'));
        addGameObject(new Enemy(160.0f, 10.0f, 3.0f, 2.0f, 'o'));
        addGameObject(new Enemy(175.0f, 10.0f, 3.0f, 2.0f, 'o'));
    }
    else if (lvl == 3) {
        addGameObject(new Brick(20.0f, 20.0f, 40.0f, 5.0f, '#'));
        addGameObject(new Brick(80.0f, 20.0f, 15.0f, 5.0f, '#'));
        addGameObject(new Brick(120.0f, 15.0f, 15.0f, 10.0f, '#'));
        addGameObject(new Brick(160.0f, 10.0f, 15.0f, 15.0f, '+'));
        addGameObject(new Enemy(25.0f, 10.0f, 3.0f, 2.0f, 'o'));
        addGameObject(new Enemy(50.0f, 10.0f, 3.0f, 2.0f, 'o'));
        addGameObject(new Enemy(80.0f, 10.0f, 3.0f, 2.0f, 'o'));
        addGameObject(new Enemy(90.0f, 10.0f, 3.0f, 2.0f, 'o'));
        addGameObject(new Enemy(120.0f, 10.0f, 3.0f, 2.0f, 'o'));
        addGameObject(new Enemy(130.0f, 10.0f, 3.0f, 2.0f, 'o'));
    }
}

void Level::restartLevel() {
    for (int i = 0; i < count; i++) {
        delete gameObjects[i];
        gameObjects[i] = nullptr;
    }
    count = 0;
    score = 0;
    delete player;
    player = new Mario(39.0f, 10.0f, 3.0f, 3.0f, '@');
    loadLevel(currentLevel);
}

void Level::removeGameObject(int index) {
    if (index < 0 || index >= count) return;
    delete gameObjects[index];
    for (int i = index; i < count - 1; i++) {
        gameObjects[i] = gameObjects[i + 1];
    }
    gameObjects[count - 1] = nullptr;
    count--;
}

void Level::handleWorldEvents() {
    if (player->hitBlock != nullptr) {
        if (player->hitBlock->getType() == '?') {
            player->hitBlock->setType('-');
            spawnCoin(player->hitBlock->getX(), player->hitBlock->getY());
        }
        player->hitBlock = nullptr;
    }

    for (int i = 0; i < count; i++) {
        if (gameObjects[i] != nullptr && gameObjects[i]->getType() == '$') {
            if (player->isCollision(*gameObjects[i])) {
                removeGameObject(i);
                score += 100;
                break;
            }
        }
    }

    for (int i = 0; i < count; i++) {
        if (gameObjects[i] == nullptr || gameObjects[i]->getType() != 'o') continue;
        if (player->isCollision(*gameObjects[i])) {
            if (player->getVertSpeed() > 0 &&
                player->getY() + player->getHeight() < gameObjects[i]->getY() + gameObjects[i]->getHeight() * 0.5f)
            {
                removeGameObject(i);
                player->setVertSpeed(-0.5f);
                score += 50;
                break;
            }
            else {
                restartLevel();
                return;
            }
        }
    }

    for (int i = 0; i < count; i++) {
        if (gameObjects[i] == nullptr || gameObjects[i]->getType() != '+') continue;
        if (player->isCollision(*gameObjects[i])) {
            currentLevel++;
            if (currentLevel > maxLevel) currentLevel = 1;
            restartLevel();
            return;
        }
    }
}

void Level::spawnCoin(float x, float y) {
    Coin* newCoin = new Coin(x, y - 1.0f, 3.0f, 2.0f, '$');
    addGameObject(newCoin);
}

void Level::updatePhysics(GameObject** all, int totalCount) {
    for (int i = 0; i < count; i++) {
        IPhysics* physicalObj = dynamic_cast<IPhysics*>(gameObjects[i]);
        if (physicalObj) {
            physicalObj->updatePhysics(levelMap, all, totalCount);
        }
    }
}

void Level::addGameObject(GameObject* obj) {
    if (count >= capacity) {
        growArray();
    }
    gameObjects[count] = obj;
    count++;
}

void Level::growArray() {
    int newCapacity = capacity * 2;
    GameObject** newArray = new GameObject*[newCapacity];
    for (int i = 0; i < newCapacity; i++) newArray[i] = nullptr;
    for (int i = 0; i < count; i++) newArray[i] = gameObjects[i];
    delete[] gameObjects;
    gameObjects = newArray;
    capacity = newCapacity;
}

bool Level::update() {
    if (!player->getIsFly() && GetKeyState(VK_SPACE) < 0) {
        player->setVertSpeed(-1.0f);
        player->setIsFly(true);
    }
    if (GetKeyState('D') < 0) {
        levelMap.scrollMap(-1.0f, player, gameObjects, count);
    }
    if (GetKeyState('A') < 0) {
        levelMap.scrollMap(1.0f, player, gameObjects, count);
    }

    if (player->getY() > 25.0f) {
        restartLevel();
        return true;
    }

    GameObject** allWithPlayer = new GameObject*[count + 1];
    for (int i = 0; i < count; i++) allWithPlayer[i] = gameObjects[i];
    allWithPlayer[count] = player;

    player->updatePhysics(levelMap, allWithPlayer, count + 1);
    updatePhysics(allWithPlayer, count + 1);

    delete[] allWithPlayer;

    handleWorldEvents();

    return true;
}

void Level::render() {
    levelMap.clear();

    for (int i = 0; i < count; i++) {
        if (gameObjects[i] != nullptr) {
            int objX = (int)std::round(gameObjects[i]->getX());
            int objY = (int)std::round(gameObjects[i]->getY());
            int objW = (int)std::round(gameObjects[i]->getWidth());
            int objH = (int)std::round(gameObjects[i]->getHeight());
            for (int x = objX; x < objX + objW; x++) {
                for (int y = objY; y < objY + objH; y++) {
                    levelMap.setChar(x, y, gameObjects[i]->getType());
                }
            }
        }
    }

    int pX = (int)std::round(player->getX());
    int pY = (int)std::round(player->getY());
    int pW = (int)std::round(player->getWidth());
    int pH = (int)std::round(player->getHeight());
    for (int x = pX; x < pX + pW; x++) {
        for (int y = pY; y < pY + pH; y++) {
            levelMap.setChar(x, y, player->getType());
        }
    }

    char scoreStr[40];
	sprintf(scoreStr, "Score: %d", score);
    int len = strlen(scoreStr);
    for (int i = 0; i < len; i++) {
        levelMap.setChar(i + 1, 0, scoreStr[i]);
    }

    levelMap.display();
}