#include "Game.h"
#include <sstream>
#include <cstdlib>
#include <ctime>
using namespace sf;
using namespace std;

void Game::initGameObjects() {
    gameObjects.push_back(keeper);
    gameObjects.push_back( make_shared<Animals>("antilope.png", 180, 155, 96, 116, "antilope", "antelope dialogue.png", "antilope.txt", 230.f));
    gameObjects.push_back( make_shared<Animals>("black cat.png", 2350, 800, 41, 52, "cat", "black cat dialogue.png", "cat.txt", 210.f));
    gameObjects.push_back( make_shared<Animals>("hedgehog.png", 1680, 870, 38, 28, "hedgehog", "hedgehog dialogue.png", "hedgehog.txt", 210.f));
    gameObjects.push_back( make_shared<Animals>("red wolf.png", 680, 850, 84, 96, "wolf", "red wolf dialogue.png", "wolf.txt", 190.f));

    struct PlantData {
         string name;
         string file;
        float x, y, w, h;
         string infoFile;
    };

     vector<PlantData> plantsData = {
        {"iva", "iva.png", 120, 100, 111, 121, "Iva_.png"},
        {"lilia", "lilia.png", 2500, 250, 22, 26, "Lilia_.png"},
        {"lotus", "lotus.png", 1330, 660, 64, 59, "Lotus_.png"},
        {"cyclamen", "cyclamenn.png", 856, 434, 28, 31, "Cyclamen_.png"},
        {"slipper", "lady's slipper.png", 2357, 567, 25, 26, "Slipper.png"},
        {"risantella", "risantella gardner.png", 200, 900, 25, 26, "Risantella.png"}
    };

    for (const auto& p : plantsData) {
        gameObjects.push_back( make_shared<Plants>(p.file, p.x, p.y, p.w, p.h, p.name, p.infoFile));
    }
}

void Game::initTasks() {
    for (const auto& item : taskInitList) {
        tasks[item.first] =  make_unique<Task>(item.second, font);
    }
}

void Game::initGameUI() {
    ui.counterTex.loadFromFile("Image/counter.png");
    ui.exitTex.loadFromFile("Image/exit_map.png");
    ui.exitLightTex.loadFromFile("Image/exit_map1.png");
    ui.holeTex.loadFromFile("Image/trash hole.png");
    ui.taskTex.loadFromFile("Image/task.png");
    ui.messageTex.loadFromFile("Image/message.png");

    ui.counterSprite.setTexture(ui.counterTex);
    ui.exitSprite.setTexture(ui.exitTex);
    ui.exitLightSprite.setTexture(ui.exitLightTex);
    ui.holeSprite.setTexture(ui.holeTex);
    ui.taskSprite.setTexture(ui.taskTex);
    ui.messageSprite.setTexture(ui.messageTex);

    ui.holeSprite.setPosition(2420, 870);

    ui.trashTextures.resize(3);
    for (int i = 0; i < 3; ++i) {
        ui.trashTextures[i].loadFromFile("Image/trash_" +  to_string(i + 1) + ".png");
    }

    for (int i = 0; i < numTrashSprites; ++i) {
        int rx =  rand() % (gameMap.getWidth() - 2) + 1;
        int ry =  rand() % (gameMap.getHeight() - 2) + 2;
         Sprite ts(ui.trashTextures[ rand() % ui.trashTextures.size()]);
        ts.setPosition(static_cast<float>(rx * 70), static_cast<float>(ry * 48));
        ui.trashSprites.push_back(ts);
    }

    ui.trashCounter.setFont(font);
    ui.trashCounter.setCharacterSize(15);
    ui.trashCounter.setFillColor( Color::White);
}

void Game::handlePlayerInput(float time) {
    if ( Keyboard::isKeyPressed( Keyboard::A)) {
        fox.dir = 1; fox.speed = 0.5f;
        currentFrame += 0.005f * time; if (currentFrame > 3) currentFrame -= 3;
        fox.sprite.setTextureRect( IntRect(138 * int(currentFrame), 492, 138, 96));
    }
    else if ( Keyboard::isKeyPressed( Keyboard::D)) {
        fox.dir = 0; fox.speed = 0.5f;
        currentFrame += 0.005f * time; if (currentFrame > 3) currentFrame -= 3;
        fox.sprite.setTextureRect( IntRect(128 * int(currentFrame), 192, 128, 96));
    }
    else if ( Keyboard::isKeyPressed( Keyboard::W)) {
        fox.dir = 3; fox.speed = 0.5f;
        currentFrame += 0.005f * time; if (currentFrame > 3) currentFrame -= 3;
        fox.sprite.setTextureRect( IntRect(135 * int(currentFrame), 332, 135, 96));
    }
    else if ( Keyboard::isKeyPressed( Keyboard::S)) {
        fox.dir = 2; fox.speed = 0.5f;
        currentFrame += 0.005f * time; if (currentFrame > 3) currentFrame -= 3;
        fox.sprite.setTextureRect( IntRect(143 * int(currentFrame), 61, 143, 96));
    }
}

void Game::handleGlobalKeysAndExit( Event event) {
     Vector2f worldPos = window.mapPixelToCoords( Mouse::getPosition(window));
    if (ui.exitSprite.getGlobalBounds().contains(worldPos) && !keeper->dialogueOpened) {
        ui.exitSprite.setTexture(ui.exitLightTex);
        if ( Mouse::isButtonPressed( Mouse::Left)) {
            view.reset( FloatRect(0, 0, static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y)));
            window.setView(view);
            menu(window, music, isSoundOn);
        }
    }
    else {
        ui.exitSprite.setTexture(ui.exitTex);
    }

    if ( Keyboard::isKeyPressed( Keyboard::Space)) {
        if (fox.messageDisplayed) {
            fox.messageDisplayed = false;
            keeper->onAllTrashCollect();
        }
        if (keeper->endTestShown) {
            keeper->endTestShown = false;
        }
    }
}

void Game::updateHUD() {
     View currentView = window.getView();
    ui.counterSprite.setPosition(currentView.getCenter().x + currentView.getSize().x / 2 - ui.counterSprite.getGlobalBounds().width, currentView.getCenter().y - currentView.getSize().y / 2);
    ui.trashCounter.setPosition(view.getCenter().x + 313, view.getCenter().y - 243);
    ui.exitSprite.setPosition(currentView.getCenter().x - currentView.getSize().x / 2, currentView.getCenter().y + currentView.getSize().y / 2 - ui.exitSprite.getGlobalBounds().height);
    ui.taskSprite.setPosition(currentView.getCenter().x - currentView.getSize().x / 2, currentView.getCenter().y - currentView.getSize().y / 2);

     stringstream ss;
    ss << "Собрано мусора: " << fox.collectedTrash << " / " << numTrashSprites;
    ui.trashCounter.setString(ss.str());
}

void Game::checkTasksCompletion() {
    bool allRed = true;
    for (auto const& pair : tasks) {
        if (pair.second->color !=  Color::Red) {
            allRed = false;
            break;
        }
    }
    if (allRed) {
        for (auto const& pair : tasks) {
            pair.second->remove();
        }
        keeper->onAllTasksCompleted();
    }
}

Game::Game()
    : window( VideoMode::getDesktopMode(), "Forsaken",  Style::Fullscreen),
    isSoundOn(true),
    fox("fox.png", 1400, 100, 100, 200),
    currentFrame(0),
    tasksVisible(true),
    answerSelected(false) {

     Image icon;
    icon.loadFromFile("Image/icon.png");
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    window.setFramerateLimit(60);

    music.openFromFile("Sound/forest.wav");
    music.play();
    menu(window, music, isSoundOn);
    view.reset( FloatRect(0, 0, 640, 480));

    font.loadFromFile("font//CyrilicOld.ttf");

    keeper =  make_shared<Keeper>("keeper.png", 1400, 100, 122, 102, "keeper", "deer dialogue.png", window, "keeper.txt");

    taskInitList = {
        {"keeper", "Взаимодействуйте с хранителем."},
        {"cat", "Взаимодействуйте с кошкой."},
        {"wolf", "Взаимодействуйте с волком."},
        {"hedgehog", "Взаимодействуйте с ежом."},
        {"antilope", "Взаимодействуйте с антилопой."},
        {"iva", "Взаимодействуйте с ивой."},
        {"lilia", "Взаимодействуйте с лилией."},
        {"lotus", "Взаимодействуйте с лотусом."},
        {"cyclamen", "Взаимодействуйте с растением цикламен."},
        {"slipper", "Взаимодействуйте с венериным башмачком."},
        {"risantella", "Взаимодействуйте с рисантелой."}
    };

    initGameObjects();
    initTasks();
    initGameUI();
}

void Game::run() {
    while (window.isOpen()) {
        float time = clock.getElapsedTime().asMicroseconds() / 800.0f;
        clock.restart();

         Event event;
        while (window.pollEvent(event)) {
            if (event.type ==  Event::Closed) window.close();

            if (event.type ==  Event::KeyPressed) {
                if (event.key.code ==  Keyboard::R) {
                    tasksVisible = !tasksVisible;
                }
                if (event.key.code ==  Keyboard::E) {
                    for (auto& obj : gameObjects) {
                        if (obj->isNear(fox)) {
                            obj->interact(fox);
                            if (tasks.find(obj->getName()) != tasks.end()) {
                                tasks[obj->getName()]->setRed();
                            }
                            break;
                        }
                    }
                }
            }
        }

        handleGlobalKeysAndExit(event);
        handlePlayerInput(time);

        getplayercoordinateforview(fox.getplayercoordinateX(), fox.getplayercoordinateY());
        fox.update(time, gameMap);
        fox.interactWithTrash(ui.trashSprites, ui.holeSprite, numTrashSprites);

        for (auto& obj : gameObjects) {
            obj->checkCollision(fox);
        }

        view.setSize(static_cast<float>(window.getSize().x) / 2.f, static_cast<float>(window.getSize().y) / 2.f);
        window.setView(view);
        window.clear();

        gameMap.draw(window);

        for (auto& obj : gameObjects) {
            obj->draw(window);
        }

        window.draw(ui.holeSprite);
        for (auto& trash : ui.trashSprites) window.draw(trash);

        window.draw(fox.sprite);
        for (auto& obj : gameObjects) {
            if (auto plant = dynamic_cast<Plants*>(obj.get())) {
                plant->drawInformWindow(window);
            }
        }

        updateHUD();

        window.draw(ui.counterSprite);
        window.draw(ui.exitSprite);
        window.draw(ui.trashCounter);
        if (tasksVisible) window.draw(ui.taskSprite);

        float startOffsetY = -250.0f, stepY = 20.0f;
        int taskIndex = 0;
        for (const auto& item : taskInitList) {
            if (tasks.find(item.first) != tasks.end()) {
                tasks[item.first]->updatePosition(window,  Vector2f(-460, startOffsetY + (taskIndex * stepY)));
                tasks[item.first]->draw(window, tasksVisible);
            }
            taskIndex++;
        }

        if (fox.messageDisplayed) {
            ui.messageSprite.setPosition(window.getView().getCenter().x - ui.messageSprite.getGlobalBounds().width / 2, window.getView().getCenter().y - ui.messageSprite.getGlobalBounds().height / 2);
            window.draw(ui.messageSprite);
        }

        for (auto& obj : gameObjects) {
            Animals* animal = dynamic_cast<Animals*>(obj.get());
            if (animal && animal != keeper.get()) {
                animal->updateDialogue();
                animal->updateDialoguePosition(window);
                animal->drawDialogue(window, 50, animal->dialogueOffsetY);
            }
        }

        keeper->updateDialoguePosition(window);
        keeper->drawKeeperDialogue(window, event, answerSelected, 50, 250);
        keeper->updateDialogue();

        checkTasksCompletion();

        if (keeper->endTestShown) {
            window.setView(keeper->uiView);
            window.draw(keeper->endTestSprite);
            window.setView(window.getView());
        }

        window.display();
    }
}