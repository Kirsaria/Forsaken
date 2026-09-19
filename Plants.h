#pragma once
#include "NPC.h"
using namespace sf;
using namespace std;

class Plants : public NPC {
public:
    bool informOpened = false;
     Image informImage;
     Texture informTexture;
     Sprite informSprite;

    Plants( String F, float X, float Y, float W, float H,  String Name,  String InformFile);

    void updateInformPosition( RenderWindow& window);
    void draw( RenderWindow& window) override;
    void drawInformWindow( RenderWindow& window);
    void interact(Player& player) override;
    void checkCollision(Player& player) override;
};