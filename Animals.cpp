#include "Animals.h"
#include <algorithm>

using namespace sf;
using namespace std;

Animals::Animals( String F, float X, float Y, float W, float H,  String Name,  String DialogueFile,  string DialogueTxtFile, float offsetY)
    : NPC(F, X, Y, W, H, Name), dialogueOffsetY(offsetY) {
    dialogueImage.loadFromFile("Image/" + DialogueFile);
    dialogueTexture.loadFromImage(dialogueImage);
    dialogueSprite.setTexture(dialogueTexture);

    playerDialogueImage.loadFromFile("Image/fox dialogue.png");
    playerDialogueTexture.loadFromImage(playerDialogueImage);
    playerDialogueSprite.setTexture(playerDialogueTexture);

    dialogueFont.loadFromFile("font//CyrilicOld.ttf");
    loadDialogueFromFile("Dialogues/" + DialogueTxtFile);
}

void Animals::loadDialogueFromFile(const  string& filename) {
     ifstream file(filename);
    if (file.is_open()) {
         string line;
        while ( getline(file, line)) {
            if (line.empty()) dialogues.push_back("");
            else if (!dialogues.empty()) dialogues.back() += line + "\n";
        }
    }
}

void Animals::updateDialogue() {
    if (dialogueOpened && letterClock.getElapsedTime().asSeconds() > letterInterval) {
        if (currentLetterIndex < dialogues[currentDialogueIndex].size()) {
            currentDialogue += dialogues[currentDialogueIndex][currentLetterIndex];
            currentLetterIndex++;
            letterClock.restart();
        }
    }
}

void Animals::drawDialogue( RenderWindow& window, float offsetX, float offsetY) {
    if (!dialogueOpened) return;

     Text dialogueText(currentDialogue, dialogueFont);
    dialogueText.setCharacterSize(24);
    dialogueText.setFillColor( Color::White);
    dialogueText.setPosition(dialogueSprite.getPosition().x + offsetX, dialogueSprite.getPosition().y + offsetY);

    if (isAnimalTurn) window.draw(dialogueSprite);
    else window.draw(playerDialogueSprite);
    window.draw(dialogueText);
}

void Animals::updateDialoguePosition( RenderWindow& window) {
     View currentView = window.getView();
     Vector2f animalPos(currentView.getCenter().x - currentView.getSize().x / 2,
        currentView.getCenter().y + currentView.getSize().y / 2 - dialogueSprite.getGlobalBounds().height);
    dialogueSprite.setPosition(animalPos);

     Vector2f playerPos(currentView.getCenter().x - currentView.getSize().x / 2,
        currentView.getCenter().y + currentView.getSize().y / 2 - playerDialogueSprite.getGlobalBounds().height);
    playerDialogueSprite.setPosition(playerPos);
}

void Animals::toggleDialogue() {
    if (dialogueClock.getElapsedTime().asSeconds() > dialogueInterval) {
        if (dialogueOpened) {
            currentDialogue = dialogues[currentDialogueIndex];
            currentLetterIndex = 0;
            currentDialogueIndex++;
            lineCounter++;
            isAnimalTurn = lineCounter % 2 == 1;
            if (currentDialogue.find('.') !=  string::npos) {
                dialogueOpened = false;
                currentDialogueIndex = 0;
                currentDialogue.clear();
            }
        }
        else {
            dialogueOpened = true;
            isAnimalTurn = true;
        }
        dialogueClock.restart();
    }
}

void Animals::checkCollision(Player& player) {
    int playerRight = player.x + player.w;
    int playerBottom = player.y + player.h;
    int npcRight = x + w;
    int npcBottom = y + h;

    int bottomCollision = npcBottom - player.y;
    int topCollision = playerBottom - y;
    int rightCollision = npcRight - player.x;
    int leftCollision = playerRight - x;

    if (bottomCollision > 0 && topCollision > 0 && rightCollision > 0 && leftCollision > 0) {
        int minOverlap =  min({ bottomCollision, topCollision, rightCollision, leftCollision });
        if (minOverlap == bottomCollision) player.y = y + h;
        else if (minOverlap == topCollision) player.y = y - player.h;
        else if (minOverlap == rightCollision && npcRight <= playerRight) player.x = x + w;
        else if (minOverlap == leftCollision && npcRight >= player.x) player.x = x - player.w;
    }
}

void Animals::interact(Player& player) {
    toggleDialogue();
}