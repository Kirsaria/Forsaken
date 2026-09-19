#include "Plants.h"
#include <cmath>
using namespace sf;
using namespace std;

Plants::Plants( String F, float X, float Y, float W, float H,  String Name,  String InformFile) : NPC(F, X, Y, W, H, Name) {
    informImage.loadFromFile("Image/" + InformFile);
    informTexture.loadFromImage(informImage);
    informSprite.setTexture(informTexture);
}

void Plants::updateInformPosition( RenderWindow& window) {
     View currentView = window.getView();
    informSprite.setPosition(currentView.getCenter().x - informSprite.getGlobalBounds().width / 2,
        currentView.getCenter().y - informSprite.getGlobalBounds().height / 2);
}

void Plants::draw( RenderWindow& window) {
    window.draw(sprite);
}

void Plants::drawInformWindow( RenderWindow& window) {
    if (informOpened) {
        updateInformPosition(window);
        window.draw(informSprite);
    }
}

void Plants::interact(Player& player) {
    informOpened = !informOpened;
}

void Plants::checkCollision(Player& player) {
    if (player.x < x + w && player.x + player.w > x && player.y < y + h && player.y + player.h > y) {
        int dx = (player.x + player.w / 2) - (x + w / 2);
        int dy = (player.y + player.h / 2) - (y + h / 2);
        if ( abs(dx) >  abs(dy)) {
            if (dx > 0) player.x = x + w;
            else player.x = x - player.w;
        }
        else {
            if (dy > 0) player.y = y + h;
            else player.y = y - player.h;
        }
    }
}