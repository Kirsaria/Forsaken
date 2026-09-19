#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <memory>
#include <map>
#include "Player.h"
#include "Keeper.h"
#include "Plants.h"
#include "Animals.h"
#include "Task.h"
#include "GameObject.h"
#include "map.h"
#include "view.h"
#include "GameMenu.h"
using namespace sf;
using namespace std;

const int numTrashSprites = 5;

struct GameUI {
     Texture counterTex, exitTex, exitLightTex, holeTex, taskTex, messageTex;
     Sprite counterSprite, exitSprite, exitLightSprite, holeSprite, taskSprite, messageSprite;
     Text trashCounter;
     vector< Texture> trashTextures;
     vector< Sprite> trashSprites;
};

class Game {
private:
     RenderWindow window;
     Music music;
    bool isSoundOn;
     Font font;
    Map gameMap;
    Player fox;
     shared_ptr<Keeper> keeper;
     vector< ::shared_ptr<IGameObject>> gameObjects;
    GameUI ui;
     Clock clock;
    float currentFrame;

    bool tasksVisible;
    bool answerSelected;
     vector< pair< string,  string>> taskInitList;
     map< string,  unique_ptr<Task>> tasks;

    void initGameObjects();
    void initTasks();
    void initGameUI();
    void handlePlayerInput(float time);
    void handleGlobalKeysAndExit( Event event);
    void updateHUD();
    void checkTasksCompletion();

public:
    Game();
    void run();
};