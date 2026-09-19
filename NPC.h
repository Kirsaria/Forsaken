#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "GameObject.h"
#include "Player.h"
using namespace sf;
using namespace std;

class NPC : public IGameObject {
public:
    float x, y, w, h;
     String name;
     Image image;
     Texture texture;
     Sprite sprite;

    NPC( String F, float X, float Y, float W, float H,  String Name);

     string getName() const override;
    void draw( RenderWindow& window) override;
    bool isNear(const Player& player) const override;
};