#include "Button.h"
using namespace sf;
using namespace std;

Button::Button( Texture& texture,  Font& font,  String buttonText,  Vector2f textPosition) {
    text.setFont(font);
    text.setString(buttonText);
    text.setCharacterSize(24);
    text.setFillColor( Color::White);
    text.setPosition(textPosition);
    buttonsprite.setTexture(texture);
}

void Button::update(const  RenderWindow& window) {
    if (buttonsprite.getGlobalBounds().contains(window.mapPixelToCoords( Mouse::getPosition(window)))) {
        text.setFillColor( Color::Red);
    }
    else {
        text.setFillColor( Color::White);
    }
}

void Button::draw( RenderWindow& window) {
    if (isVisible) {
        window.draw(buttonsprite);
        window.draw(text);
    }
}

bool Button::Pressed( RenderWindow& window,  Event event) {
     Vector2i mousePosition =  Mouse::getPosition(window);
    if (event.type ==  Event::MouseButtonPressed && event.mouseButton.button ==  Mouse::Left) {
        if (buttonsprite.getGlobalBounds().contains(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y))) {
            return true;
        }
    }
    return false;
}