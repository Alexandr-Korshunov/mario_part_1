#pragma once
class Map;
class GameObject;

class IPhysics {
public:
    virtual ~IPhysics() {} 
    virtual void updatePhysics(Map& map, GameObject** allObjects, int count) = 0;
};