#include "Keeper.h"
using namespace sf;
using namespace std;

Keeper::Keeper( String F, float X, float Y, float W, float H,  String Name,  String DialogueFile,  RenderWindow& window,  string DialogueTxtFile)
    : Animals(F, X, Y, W, H, Name, DialogueFile, DialogueTxtFile, 250.f),
    yesButton(buttonTexture, buttonFont, "Да", { 1300, 940 }),
    noButton(buttonTexture, buttonFont, "Нет", { 1640, 940 }),
    option1Button(buttonTexture, buttonFont, "", { 680, 940 }),
    option2Button(buttonTexture, buttonFont, "", { 960, 940 }),
    option3Button(buttonTexture, buttonFont, "", { 1280, 940 }),
    option4Button(buttonTexture, buttonFont, "", { 1577, 940 }) {
    uiView.setSize(static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y));
    uiView.setCenter(static_cast<float>(window.getSize().x) / 2.f, static_cast<float>(window.getSize().y) / 2.f);
    buttonTexture.loadFromFile("Image/Answer.png");
    buttonFont.loadFromFile("font//CyrilicOld.ttf");
}

void Keeper::onAllTasksCompleted() { TaskComplete = true; }
void Keeper::onAllTrashCollect() { TrashComplete = true; }

void Keeper::loadCurrentQuestionOptions() {
     ifstream testFile("Dialogues/test.txt");
     string line;
     vector< string> lines;
    while ( getline(testFile, line)) lines.push_back(line);

    if (lines.empty()) return;
    int randomIndex = ( rand() % (lines.size() / 6)) * 6;
    currentDialogue = lines[randomIndex];
    option1Button.text.setString(lines[randomIndex + 1]);
    option2Button.text.setString(lines[randomIndex + 2]);
    option3Button.text.setString(lines[randomIndex + 3]);
    option4Button.text.setString(lines[randomIndex + 4]);
    correctAnswerIndex =  stoi(lines[randomIndex + 5]);
}

void Keeper::drawKeeperDialogue( RenderWindow& window,  Event event, bool& answerSelectedRef, float offsetX, float offsetY) {
    drawDialogue(window, offsetX, offsetY);
    if (!dialogueOpened) return;

     View currentView = window.getView();
    window.setView(uiView);
    if (TaskComplete && TrashComplete && !questionAsked) {
        yesButton.draw(window); yesButton.update(window);
        noButton.draw(window); noButton.update(window);
        if (noButton.Pressed(window, event)) dialogueOpened = false;
        if (yesButton.Pressed(window, event)) {
            yesButtonPressed = true;
            questionAsked = true;
            loadCurrentQuestionOptions();
        }
    }
    if (yesButtonPressed) {
        option1Button.draw(window); option1Button.update(window);
        option2Button.draw(window); option2Button.update(window);
        option3Button.draw(window); option3Button.update(window);
        option4Button.draw(window); option4Button.update(window);

        if (option1Button.Pressed(window, event)) answerPressed = (correctAnswerIndex == 1);
        else if (option2Button.Pressed(window, event)) answerPressed = (correctAnswerIndex == 2);
        else if (option3Button.Pressed(window, event)) answerPressed = (correctAnswerIndex == 3);
        else if (option4Button.Pressed(window, event)) answerPressed = (correctAnswerIndex == 4);

        if (answerPressed) {
            answerSelectedRef = true;
            if (currentQuestionIndex < totalQuestions - 1) {
                currentQuestionIndex++;
                loadCurrentQuestionOptions();
                answerPressed = false;
            }
            else {
                dialogueOpened = false;
                yesButtonPressed = false;
                questionAsked = false;
                currentQuestionIndex = 0;
                endTestTexture.loadFromFile("Image/message_end.png");
                endTestShown = true;
                endTestSprite.setTexture(endTestTexture);
                endTestSprite.setPosition(static_cast<float>(window.getSize().x) / 2.f - static_cast<float>(endTestTexture.getSize().x) / 2.f,
                    static_cast<float>(window.getSize().y) / 2.f - static_cast<float>(endTestTexture.getSize().y) / 2.f);
            }
        }
    }
    window.setView(currentView);
}