#include "NPC.h"
#include <cmath>
using namespace sf;
using namespace std;

NPC::NPC( String F, float X, float Y, float W, float H,  String Name)
    : x(X), y(Y), w(W), h(H), name(Name) {
    image.loadFromFile("Image/" + F);
    texture.loadFromImage(image);
    sprite.setTexture(texture);
    sprite.setPosition(x, y);
}

 string NPC::getName() const {
    return name;
}

void NPC::draw( RenderWindow& window) {
    window.draw(sprite);
}

bool NPC::isNear(const Player& player) const {
    return  abs(player.x - x) < 150 &&  abs(player.y - y) < 150;
}