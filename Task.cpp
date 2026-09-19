#include "Task.h"
using namespace sf;
using namespace std;

Task::Task(const  string& description, const  Font& font) : description(description) {
    taskText.setFont(font);
    taskText.setString(description);
    taskText.setCharacterSize(18);
    taskText.setFillColor( Color::White);
}

void Task::remove() {
    isRemoved = true;
}

void Task::updatePosition( RenderWindow& window, const  Vector2f& position) {
     View currentView = window.getView();
    taskText.setPosition(currentView.getCenter().x + position.x, currentView.getCenter().y + position.y);
}

void Task::draw( RenderWindow& window, bool tasksVisible) {
    if (!isRemoved && tasksVisible) {
        window.draw(taskText);
    }
}

void Task::setRed() {
    color =  Color::Red;
    taskText.setFillColor(color);
}