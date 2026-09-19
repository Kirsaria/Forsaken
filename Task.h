#pragma once
#include <SFML/Graphics.hpp>
#include <string>
using namespace sf;
using namespace std;

class Task {
private:
    bool isRemoved = false;
public:
     string description;
     Text taskText;
     Color color =  Color::White;

    Task(const  string& description, const  Font& font);
    void remove();
    void updatePosition(RenderWindow& window, const  Vector2f& position);
    void draw( RenderWindow& window, bool tasksVisible);
    void setRed();
};