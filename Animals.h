#pragma once
#include "NPC.h"
#include <vector>
#include <fstream>
using namespace sf;
using namespace std;

class Animals : public NPC {
public:
    bool dialogueOpened = false;
     Image dialogueImage, playerDialogueImage;
     Texture dialogueTexture, playerDialogueTexture;
     Sprite dialogueSprite, playerDialogueSprite;
     vector< string> dialogues;
    int currentDialogueIndex = 0;
     string currentDialogue = "";
    int currentLetterIndex = 0;
     Clock letterClock;
    float letterInterval = 0.05f;
    bool isAnimalTurn = true;
     Clock dialogueClock;
    float dialogueInterval = 0.5f;
    int lineCounter = 0;
     Font dialogueFont;
    float dialogueOffsetY;

    Animals( String F, float X, float Y, float W, float H,  String Name,  String DialogueFile,  string DialogueTxtFile, float offsetY = 210.f);

    void loadDialogueFromFile(const  string& filename);
    virtual void updateDialogue();
    virtual void drawDialogue( RenderWindow& window, float offsetX = 0, float offsetY = 0);
    void updateDialoguePosition( RenderWindow& window);
    virtual void toggleDialogue();
    void checkCollision(Player& player) override;
    void interact(Player& player) override;

    virtual void onAllTasksCompleted() {}
    virtual void onAllTrashCollect() {}
};