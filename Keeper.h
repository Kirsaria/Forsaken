#pragma once
#include "Animals.h"
#include "Button.h"
using namespace sf;
using namespace std;

class Keeper : public Animals {
public:
    bool TaskComplete = false;
    bool TrashComplete = false;
    bool questionAsked = false;
    bool yesButtonPressed = false;
    int currentQuestionIndex = 0;
    int correctAnswerIndex = 0;
    const int totalQuestions = 5;
    bool answerPressed = false;
     Texture buttonTexture;
     Font buttonFont;
    Button yesButton, noButton, option1Button, option2Button, option3Button, option4Button;
     View uiView;
     Texture endTestTexture;
     Sprite endTestSprite;
    bool endTestShown = false;

    Keeper( String F, float X, float Y, float W, float H,  String Name,  String DialogueFile,  RenderWindow& window,  string DialogueTxtFile);

    void onAllTasksCompleted() override;
    void onAllTrashCollect() override;
    void loadCurrentQuestionOptions();
    void drawKeeperDialogue( RenderWindow& window,  Event event, bool& answerSelectedRef, float offsetX = 0, float offsetY = 0);
};