#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>

using namespace sf;

void InitText(Text& mtext, float xpos, float ypos, String str, int size_font,
    Color menu_text_color = Color::White, int bord = 0, Color border_color = Color::Black);

void menu(RenderWindow& window, sf::Music& music, bool& isSoundOn) {
    std::vector<Texture> foxTextures(4);
    for (int i = 0; i < 4; ++i) {
        foxTextures[i].loadFromFile("Image/foxgif_" + std::to_string(i + 1) + ".png");
    }

    std::string buttonNames[3] = { "play", "tutor", "exit" };
    std::vector<Texture> menuTextures(3);
    std::vector<Texture> menuLights(3);
    for (int i = 0; i < 3; ++i) {
        menuTextures[i].loadFromFile("Image/" + buttonNames[i] + "_1.png");
        menuLights[i].loadFromFile("Image/" + buttonNames[i] + "_2.png");
    }

    Texture soundOnTex, soundOffTex, tutorTexture, newTexture;
    soundOnTex.loadFromFile("Image/sound on.png");
    soundOffTex.loadFromFile("Image/sound off.png");
    tutorTexture.loadFromFile("Image/tutor.png");
    newTexture.loadFromFile("Image/tutor_W.png");

    std::vector<Sprite> menuButtons(3);
    for (int i = 0; i < 3; ++i) {
        menuButtons[i].setTexture(menuTextures[i]);
        menuButtons[i].setPosition(100, 200 + i * 300); 
    }

    Sprite soundButton;
    soundButton.setTexture(isSoundOn ? soundOnTex : soundOffTex);
    soundButton.setPosition(1600, 800);

    Sprite foxSprite;
    foxSprite.setPosition(800, 600);
    foxSprite.setTexture(foxTextures[0]);
    foxSprite.setScale(5, 5);

    Sprite tutor(tutorTexture);

    float width = VideoMode::getDesktopMode().width;
    float height = VideoMode::getDesktopMode().height;
    RectangleShape background(Vector2f(width, height));
    Texture texture_window;
    texture_window.loadFromFile("Image/background.png");
    background.setTexture(&texture_window);

    Font font;
    font.loadFromFile("font/monotypecorsiva.ttf");
    Text Titul;
    Titul.setFont(font);
    InitText(Titul, 750, 40, "Forsaken", 250, Color(76, 47, 39), 2);

    Clock clock;
    float speed = 0.1f;
    bool isMenu = true;

    while (isMenu && window.isOpen()) {
        int index = static_cast<int>(clock.getElapsedTime().asSeconds() / speed) % foxTextures.size();
        foxSprite.setTexture(foxTextures[index]);

        Vector2i mousePosition = Mouse::getPosition(window);

        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
                isMenu = false;
            }

            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                if (menuButtons[0].getGlobalBounds().contains(mousePosition.x, mousePosition.y)) {
                    std::cout << "Menu 1 selected" << std::endl;
                    isMenu = false;
                }
                else if (menuButtons[1].getGlobalBounds().contains(mousePosition.x, mousePosition.y)) {
                    std::cout << "Menu 2 selected" << std::endl;

                    bool isTutor = true;
                    while (isTutor && window.isOpen()) {
                        Event tutorEvent;
                        while (window.pollEvent(tutorEvent)) {
                            if (tutorEvent.type == Event::Closed) {
                                window.close();
                                isTutor = false;
                            }
                            if (tutorEvent.type == Event::KeyPressed && tutorEvent.key.code == Keyboard::Escape) {
                                isTutor = false;
                            }
                        }
                        window.clear();
                        window.draw(tutor);
                        window.display();
                    }
                }
                else if (menuButtons[2].getGlobalBounds().contains(mousePosition.x, mousePosition.y)) {
                    std::cout << "Menu 3 selected" << std::endl;
                    window.close();
                    isMenu = false;
                }
                else if (soundButton.getGlobalBounds().contains(mousePosition.x, mousePosition.y)) {
                    isSoundOn = !isSoundOn;

                    if (isSoundOn) {
                        soundButton.setTexture(soundOnTex);
                        if (music.getStatus() != sf::Music::Playing)
                            music.play();
                    }
                    else {
                        soundButton.setTexture(soundOffTex);
                        if (music.getStatus() != sf::Music::Paused)
                            music.pause();
                    }
                }
            }
        }

        for (int i = 0; i < 3; ++i) {
            if (menuButtons[i].getGlobalBounds().contains(mousePosition.x, mousePosition.y)) {
                menuButtons[i].setTexture(menuLights[i]);
            }
            else {
                menuButtons[i].setTexture(menuTextures[i]);
            }
        }

        window.clear();
        window.draw(background);
        window.draw(Titul);
        for (int i = 0; i < 3; ++i) {
            window.draw(menuButtons[i]);
        }
        window.draw(soundButton);
        window.draw(foxSprite);
        window.display();
    }
}

void InitText(Text& mtext, float xpos, float ypos, String str, int size_font,
    Color menu_text_color, int bord, Color border_color)
{
    mtext.setCharacterSize(size_font);
    mtext.setPosition(xpos, ypos);
    mtext.setString(str);
    mtext.setFillColor(menu_text_color);
    mtext.setOutlineThickness(bord);
    mtext.setOutlineColor(border_color);
}