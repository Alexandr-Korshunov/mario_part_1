#include "Map.hpp"
#include "Mario.hpp"
#include "GameObject.hpp"

class Level {
private:
    Map levelMap;
    Mario* player;
    GameObject** gameObjects;
    int capacity;
    int count;
    int currentLevel;
    int maxLevel;
    int score;

    void updatePhysics(GameObject** all, int totalCount);
    void growArray();
    void loadLevel(int lvl);

public:
    Level(int width, int height);
    ~Level();

    void restartLevel();
    void removeGameObject(int index);
    void handleWorldEvents();
    void spawnCoin(float x, float y);
    void addGameObject(GameObject* obj);
    bool update();
    void render();
};