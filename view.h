#include <SFML/Graphics.hpp>
using namespace sf;
sf::View view;

void getplayercoordinateforview(float x, float y)
{ 
	float tempX = x; float tempY = y;
	if (x < 500) tempX = 500;
	if (x > 2320) tempX = 2320;
	if (y < 270) tempY = 270;
	if (y > 854) tempY = 854;
	view.setCenter(tempX, tempY); 
}
