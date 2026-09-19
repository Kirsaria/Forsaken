#pragma once
#include <SFML/Graphics.hpp>
#include <string>
using namespace sf;
using namespace std;

class Player;

class IGameObject {
public:
    virtual ~IGameObject() = default;
    virtual void update(float time) {}
    virtual void draw( RenderWindow& window) = 0;
    virtual void checkCollision(Player& player) = 0;
    virtual void interact(Player& player) {}
    virtual  string getName() const = 0;
    virtual bool isNear(const Player& player) const = 0;
};