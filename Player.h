#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "map.h"
using namespace sf;
using namespace std;

class Player {
public:
    float x, y, w, h, dx, dy, speed;
    int dir = 0;
     String File;
     Image image;
     Texture texture;
     Sprite sprite;
    int collectedTrash = 0;
    int heldTrashIndex = -1;
    bool allTrashCollected = false;
    bool messageDisplayed = false;

    Player( String F, float X, float Y, float W, float H);

    void update(float time, const Map& gameMap);
    void interactionWithMap(const Map& gameMap);
    void interactWithTrash( vector< Sprite>& trashSprites,  Sprite& holeSprite, int numTrashSprites);

    float getplayercoordinateX();
    float getplayercoordinateY();
};