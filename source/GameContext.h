#pragma once
#include "Maze.h"

class Pacman : public IEntity{
public:
    void move(Direction direction);
};

class IStaticEntity : public IEntity{
public:
    virtual IStaticEntity* clone();
};

class IDynamicEntity : public IEntity{
public:
    virtual IDynamicEntity* clone();
    virtual void action();
};

class Enemy : public IDynamicEntity{

};

class Food : public IStaticEntity{

};

class State
{

};
class GameContext{
public:
    Pacman* pacman;
    std::vector<IStaticEntity*> static_objects;
    std::vector<IDynamicEntity*> dynamic_objects;
    State state;
public:
    GameContext clone();
};