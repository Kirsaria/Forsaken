#include "Player.h"
#include <cmath>

using namespace sf;
using namespace std;

Player::Player( String F, float X, float Y, float W, float H) {
    dx = 0; dy = 0; speed = 0;
    File = F;
    w = W; h = H;
    image.loadFromFile("Image/" + File);
    texture.loadFromImage(image);
    sprite.setTexture(texture);
    x = X; y = Y;
    sprite.setTextureRect( IntRect(0, 0, w, h));
}

void Player::update(float time, const Map& gameMap) {
    switch (dir) {
    case 0: dx = speed; dy = 0; break;
    case 1: dx = -speed; dy = 0; break;
    case 2: dx = 0; dy = speed; break;
    case 3: dx = 0; dy = -speed; break;
    }
    x += dx * time;
    y += dy * time;
    speed = 0;
    sprite.setPosition(x, y);
    interactionWithMap(gameMap);
}

void Player::interactionWithMap(const Map& gameMap) {
    int height = 48;
    int width = 70;
    for (int i = y / height; i < (y + h) / height; i++)
        for (int j = x / 70; j < (x + w) / 70; j++) {
            if (gameMap.isCollisionTile(i, j)) {
                if (dy > 0) { y = i * height - h; }
                if (dy < 0) { y = i * height + height; }
                if (dx > 0) { x = j * width - w; }
                if (dx < 0) { x = j * width + width; }
            }
        }
}

void Player::interactWithTrash( vector< Sprite>& trashSprites,  Sprite& holeSprite, int numTrashSprites) {
    if (heldTrashIndex != -1 && heldTrashIndex < static_cast<int>(trashSprites.size())) {
        trashSprites[heldTrashIndex].setPosition(x, y);
         FloatRect holeBounds = holeSprite.getGlobalBounds();
         Vector2f trashCenter = trashSprites[heldTrashIndex].getPosition() +
             Vector2f(trashSprites[heldTrashIndex].getGlobalBounds().width / 2,
                trashSprites[heldTrashIndex].getGlobalBounds().height / 2);

        if (holeBounds.contains(trashCenter)) {
            trashSprites.erase(trashSprites.begin() + heldTrashIndex);
            heldTrashIndex = -1;
            collectedTrash++;
        }
    }
    else {
        for (size_t i = 0; i < trashSprites.size(); ++i) {
            float distance =  sqrt( pow(x - trashSprites[i].getPosition().x, 2) +
                 pow(y - trashSprites[i].getPosition().y, 2));

            if (distance < 30) {
                heldTrashIndex = static_cast<int>(i);
                break;
            }
        }
    }

    if (collectedTrash == numTrashSprites && !allTrashCollected) {
        allTrashCollected = true;
        messageDisplayed = true;
    }
}

float Player::getplayercoordinateX() { return x; }
float Player::getplayercoordinateY() { return y; }