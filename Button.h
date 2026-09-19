#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;
using namespace std;

class Button {
public:
     Sprite buttonsprite;
     Text text;
    bool isVisible = true;

    Button( Texture& texture,  Font& font,  String buttonText,  Vector2f textPosition);

    void update(const  RenderWindow& window);
    void draw( RenderWindow& window);
    bool Pressed( RenderWindow& window,  Event event);
};