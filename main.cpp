#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <memory>
#include <random>
#include <ctime>
#include "GameMenu.h"
#include "map.h" 
#include "view.h"

using namespace sf;
using namespace std;

const int numTrashSprites = 5;
bool tasksVisible = true;
bool answerSelected = false;

class Player;

class IGameObject {
public:
    virtual ~IGameObject() = default;
    virtual void update(float time) {}
    virtual void draw(RenderWindow& window) = 0;
    virtual void checkCollision(class Player& player) = 0;
    virtual void interact(Player& player) {}
    virtual string getName() const = 0;
    virtual bool isNear(const Player& player) const = 0;
};

class Task {
private:
    bool isRemoved = false;
public:
    string description;
    Text taskText;
    Color color = Color::White;

    Task(const string& description, const Font& font) : description(description) {
        taskText.setFont(font);
        taskText.setString(description);
        taskText.setCharacterSize(18);
        taskText.setFillColor(Color::White);
    }

    void remove() { isRemoved = true; }

    void updatePosition(RenderWindow& window, const Vector2f& position) {
        View currentView = window.getView();
        taskText.setPosition(currentView.getCenter().x + position.x, currentView.getCenter().y + position.y);
    }

    void draw(RenderWindow& window) {
        if (!isRemoved && tasksVisible) {
            window.draw(taskText);
        }
    }

    void setRed() {
        color = Color::Red;
        taskText.setFillColor(color);
    }
};

map<string, unique_ptr<Task>> tasks;

class Player {
public:
    float x, y, w, h, dx, dy, speed;
    int dir = 0;
    String File;
    Image image;
    Texture texture;
    Sprite sprite;
    int collectedTrash = 0;
    int heldTrashIndex = -1;
    bool allTrashCollected = false;
    bool messageDisplayed = false;

    Player(String F, float X, float Y, float W, float H) {
        dx = 0; dy = 0; speed = 0;
        File = F;
        w = W; h = H;
        image.loadFromFile("Image/" + File);
        texture.loadFromImage(image);
        sprite.setTexture(texture);
        x = X; y = Y;
        sprite.setTextureRect(IntRect(0, 0, w, h));
    }

    void update(float time, const Map& gameMap) {
        switch (dir) {
        case 0: dx = speed; dy = 0; break;
        case 1: dx = -speed; dy = 0; break;
        case 2: dx = 0; dy = speed; break;
        case 3: dx = 0; dy = -speed; break;
        }
        x += dx * time;
        y += dy * time;
        speed = 0;
        sprite.setPosition(x, y);
        interactionWithMap(gameMap);
    }

    void interactionWithMap(const Map& gameMap) {
        int height = 48;
        int width = 70;
        for (int i = y / height; i < (y + h) / height; i++)
            for (int j = x / 70; j < (x + w) / 70; j++) {
                if (gameMap.isCollisionTile(i, j)) {
                    if (dy > 0) { y = i * height - h; }
                    if (dy < 0) { y = i * height + height; }
                    if (dx > 0) { x = j * width - w; }
                    if (dx < 0) { x = j * width + width; }
                }
            }
    }

    void interactWithTrash(vector<Sprite>& trashSprites, Sprite& holeSprite) {
        if (heldTrashIndex != -1 && heldTrashIndex < static_cast<int>(trashSprites.size())) {
            trashSprites[heldTrashIndex].setPosition(x, y);
            FloatRect holeBounds = holeSprite.getGlobalBounds();
            Vector2f trashCenter = trashSprites[heldTrashIndex].getPosition() +
                Vector2f(trashSprites[heldTrashIndex].getGlobalBounds().width / 2,
                    trashSprites[heldTrashIndex].getGlobalBounds().height / 2);

            if (holeBounds.contains(trashCenter)) {
                trashSprites.erase(trashSprites.begin() + heldTrashIndex);
                heldTrashIndex = -1;
                collectedTrash++;
            }
        }
        else {
            for (size_t i = 0; i < trashSprites.size(); ++i) {
                float distance = sqrt(pow(x - trashSprites[i].getPosition().x, 2) +
                    pow(y - trashSprites[i].getPosition().y, 2));
                if (distance < 30) {
                    heldTrashIndex = static_cast<int>(i);
                    break;
                }
            }
        }

        if (collectedTrash == numTrashSprites && !allTrashCollected) {
            allTrashCollected = true;
            messageDisplayed = true;
        }
    }

    float getplayercoordinateX() { return x; }
    float getplayercoordinateY() { return y; }
};

class NPC : public IGameObject {
public:
    float x, y, w, h;
    String name;
    Image image;
    Texture texture;
    Sprite sprite;

    NPC(String F, float X, float Y, float W, float H, String Name)
        : x(X), y(Y), w(W), h(H), name(Name) {
        image.loadFromFile("Image/" + F);
        texture.loadFromImage(image);
        sprite.setTexture(texture);
        sprite.setPosition(x, y);
    }

    string getName() const override { return name; }
    void draw(RenderWindow& window) override { window.draw(sprite); }

    bool isNear(const Player& player) const override {
        return abs(player.x - x) < 150 && abs(player.y - y) < 150;
    }
};

class Animals : public NPC {
public:
    bool dialogueOpened = false;
    Image dialogueImage, playerDialogueImage;
    Texture dialogueTexture, playerDialogueTexture;
    Sprite dialogueSprite, playerDialogueSprite;
    vector<string> dialogues;
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

    Animals(String F, float X, float Y, float W, float H, String Name, String DialogueFile, string DialogueTxtFile, float offsetY = 210.f)
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

    void loadDialogueFromFile(const string& filename) {
        ifstream file(filename);
        if (file.is_open()) {
            string line;
            while (getline(file, line)) {
                if (line.empty()) dialogues.push_back("");
                else if (!dialogues.empty()) dialogues.back() += line + "\n";
            }
        }
    }

    virtual void updateDialogue() {
        if (dialogueOpened && letterClock.getElapsedTime().asSeconds() > letterInterval) {
            if (currentLetterIndex < dialogues[currentDialogueIndex].size()) {
                currentDialogue += dialogues[currentDialogueIndex][currentLetterIndex];
                currentLetterIndex++;
                letterClock.restart();
            }
        }
    }

    virtual void drawDialogue(RenderWindow& window, float offsetX = 0, float offsetY = 0) {
        if (!dialogueOpened) return;

        Text dialogueText(currentDialogue, dialogueFont);
        dialogueText.setCharacterSize(24);
        dialogueText.setFillColor(Color::White);
        dialogueText.setPosition(dialogueSprite.getPosition().x + offsetX, dialogueSprite.getPosition().y + offsetY);

        if (isAnimalTurn) window.draw(dialogueSprite);
        else window.draw(playerDialogueSprite);
        window.draw(dialogueText);
    }

    void updateDialoguePosition(RenderWindow& window) {
        View currentView = window.getView();

        Vector2f animalPos(currentView.getCenter().x - currentView.getSize().x / 2,
            currentView.getCenter().y + currentView.getSize().y / 2 - dialogueSprite.getGlobalBounds().height);
        dialogueSprite.setPosition(animalPos);

        Vector2f playerPos(currentView.getCenter().x - currentView.getSize().x / 2,
            currentView.getCenter().y + currentView.getSize().y / 2 - playerDialogueSprite.getGlobalBounds().height);
        playerDialogueSprite.setPosition(playerPos);
    }

    virtual void toggleDialogue() {
        if (dialogueClock.getElapsedTime().asSeconds() > dialogueInterval) {
            if (dialogueOpened) {
                currentDialogue = dialogues[currentDialogueIndex];
                currentLetterIndex = 0;
                currentDialogueIndex++;
                lineCounter++;
                isAnimalTurn = lineCounter % 2 == 1;
                if (currentDialogue.find('.') != string::npos) {
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

    void checkCollision(Player& player) override {
        int playerRight = player.x + player.w;
        int playerBottom = player.y + player.h;
        int npcRight = x + w;
        int npcBottom = y + h;

        int bottomCollision = npcBottom - player.y;
        int topCollision = playerBottom - y;
        int rightCollision = npcRight - player.x;
        int leftCollision = playerRight - x;

        if (bottomCollision > 0 && topCollision > 0 && rightCollision > 0 && leftCollision > 0) {
            int minOverlap = min({ bottomCollision, topCollision, rightCollision, leftCollision });
            if (minOverlap == bottomCollision) player.y = y + h;
            else if (minOverlap == topCollision) player.y = y - player.h;
            else if (minOverlap == rightCollision && npcRight <= playerRight) player.x = x + w;
            else if (minOverlap == leftCollision && npcRight >= player.x) player.x = x - player.w;
        }
    }

    void interact(Player& player) override {
        toggleDialogue();
        if (tasks.find(name) != tasks.end()) {
            tasks[name]->setRed();
        }
    }

    virtual void onAllTasksCompleted() {}
    virtual void onAllTrashCollect() {}
};

class Button {
public:
    Sprite buttonsprite;
    Text text;
    bool isVisible = true;

    Button(Texture& texture, Font& font, String buttonText, Vector2f textPosition) {
        text.setFont(font);
        text.setString(buttonText);
        text.setCharacterSize(24);
        text.setFillColor(Color::White);
        text.setPosition(textPosition);
        buttonsprite.setTexture(texture);
    }

    void update(const RenderWindow& window) {
        if (buttonsprite.getGlobalBounds().contains(window.mapPixelToCoords(Mouse::getPosition(window)))) {
            text.setFillColor(Color::Red);
        }
        else {
            text.setFillColor(Color::White);
        }
    }

    void draw(RenderWindow& window) {
        if (isVisible) {
            window.draw(buttonsprite);
            window.draw(text);
        }
    }

    bool Pressed(RenderWindow& window, Event event) {
        Vector2i mousePosition = Mouse::getPosition(window);
        if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
            if (buttonsprite.getGlobalBounds().contains(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y))) {
                return true;
            }
        }
        return false;
    }
};

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

    Keeper(String F, float X, float Y, float W, float H, String Name, String DialogueFile, RenderWindow& window, string DialogueTxtFile)
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

    void onAllTasksCompleted() override { TaskComplete = true; }
    void onAllTrashCollect() override { TrashComplete = true; }

    void loadCurrentQuestionOptions() {
        ifstream testFile("Dialogues/test.txt");
        string line;
        vector<string> lines;
        while (getline(testFile, line)) lines.push_back(line);

        if (lines.empty()) return;
        int randomIndex = (rand() % (lines.size() / 6)) * 6;
        currentDialogue = lines[randomIndex];
        option1Button.text.setString(lines[randomIndex + 1]);
        option2Button.text.setString(lines[randomIndex + 2]);
        option3Button.text.setString(lines[randomIndex + 3]);
        option4Button.text.setString(lines[randomIndex + 4]);
        correctAnswerIndex = stoi(lines[randomIndex + 5]);
    }

    void drawKeeperDialogue(RenderWindow& window, Event event, float offsetX = 0, float offsetY = 0) {
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
                answerSelected = true;
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
};

class Plants : public NPC {
public:
    bool informOpened = false;
    Image informImage;
    Texture informTexture;
    Sprite informSprite;

    Plants(String F, float X, float Y, float W, float H, String Name, String InformFile) : NPC(F, X, Y, W, H, Name) {
        informImage.loadFromFile("Image/" + InformFile);
        informTexture.loadFromImage(informImage);
        informSprite.setTexture(informTexture);
    }

    void updateInformPosition(RenderWindow& window) {
        View currentView = window.getView();
        informSprite.setPosition(currentView.getCenter().x - informSprite.getGlobalBounds().width / 2,
            currentView.getCenter().y - informSprite.getGlobalBounds().height / 2);
    }

    void draw(RenderWindow& window) override {
        window.draw(sprite);
    }

    void drawInformWindow(RenderWindow& window) {
        if (informOpened) {
            updateInformPosition(window);
            window.draw(informSprite);
        }
    }

    void interact(Player& player) override {
        informOpened = !informOpened;
        if (tasks.find(name) != tasks.end()) {
            tasks[name]->setRed();
        }
    }

    void checkCollision(Player& player) override {
        if (player.x < x + w && player.x + player.w > x && player.y < y + h && player.y + player.h > y) {
            int dx = (player.x + player.w / 2) - (x + w / 2);
            int dy = (player.y + player.h / 2) - (y + h / 2);
            if (abs(dx) > abs(dy)) {
                if (dx > 0) player.x = x + w;
                else player.x = x - player.w;
            }
            else {
                if (dy > 0) player.y = y + h;
                else player.y = y - player.h;
            }
        }
    }
};

int main() {
    system("chcp 1251");
    RenderWindow window(VideoMode::getDesktopMode(), "Forsaken", Style::Fullscreen);
    Image icon;
    icon.loadFromFile("Image/icon.png");
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    window.setFramerateLimit(60);

    bool isSoundOn = true;
    Music music;
    music.openFromFile("Sound/forest.wav");
    music.play();
    menu(window, music, isSoundOn);
    view.reset(FloatRect(0, 0, 640, 480));

    Font font;
    font.loadFromFile("font//CyrilicOld.ttf");

    Map gameMap;

    Player fox("fox.png", 1400, 100, 100, 200);

    auto keeper = make_shared<Keeper>("keeper.png", 1400, 100, 122, 102, "keeper", "deer dialogue.png", window, "keeper.txt");

    vector<shared_ptr<IGameObject>> gameObjects;
    gameObjects.push_back(keeper);
    gameObjects.push_back(make_shared<Animals>("antilope.png", 180, 155, 96, 116, "antilope", "antelope dialogue.png", "antilope.txt", 230.f));
    gameObjects.push_back(make_shared<Animals>("black cat.png", 2350, 800, 41, 52, "cat", "black cat dialogue.png", "cat.txt", 210.f));
    gameObjects.push_back(make_shared<Animals>("hedgehog.png", 1680, 870, 38, 28, "hedgehog", "hedgehog dialogue.png", "hedgehog.txt", 210.f));
    gameObjects.push_back(make_shared<Animals>("red wolf.png", 680, 850, 84, 96, "wolf", "red wolf dialogue.png", "wolf.txt", 190.f));

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
        gameObjects.push_back(make_shared<Plants>(p.file, p.x, p.y, p.w, p.h, p.name, p.infoFile));
    }

    vector<pair<string, string>> taskInitList = {
        {"keeper", "Взаимодействуйте с хранителем."}, {"cat", "Взаимодействуйте с кошкой."},
        {"wolf", "Взаимодействуйте с волком."}, {"hedgehog", "Взаимодействуйте с ежом."},
        {"antilope", "Взаимодействуйте с антилопой."}, {"iva", "Взаимодействуйте с ивой."},
        {"lilia", "Взаимодействуйте с лилией."}, {"lotus", "Взаимодействуйте с лотусом."},
        {"cyclamen", "Взаимодействуйте с растением цикламен."}, {"slipper", "Взаимодействуйте с венериным башмачком."},
        {"risantella", "Взаимодействуйте с рисантелой."}
    };
    for (const auto& item : taskInitList) {
        tasks[item.first] = make_unique<Task>(item.second, font);
    }

    Clock clock;
    float CurrentFrame = 0;

    Text trashCounter("", font, 15);
    trashCounter.setFillColor(Color::White);
    Texture CounterTexture, ExitTexture, ExitLightTexture, holeTexture, TaskTexture, messageTexture;
    CounterTexture.loadFromFile("Image/counter.png");
    ExitTexture.loadFromFile("Image/exit_map.png");
    ExitLightTexture.loadFromFile("Image/exit_map1.png");
    holeTexture.loadFromFile("Image/trash hole.png");
    TaskTexture.loadFromFile("Image/task.png");
    messageTexture.loadFromFile("Image/message.png");

    Sprite CounterSprite(CounterTexture), ExitSprite(ExitTexture), ExitLightSprite(ExitLightTexture), holeSprite(holeTexture), TaskSprite(TaskTexture), message(messageTexture);
    holeSprite.setPosition(2420, 870);

    vector<Texture> trashTextures(3);
    for (int i = 0; i < 3; ++i) {
        trashTextures[i].loadFromFile("Image/trash_" + to_string(i + 1) + ".png");
    }

    vector<Sprite> trashSprites;
    for (int i = 0; i < numTrashSprites; ++i) {
        int x = rand() % (gameMap.getWidth() - 2) + 1;
        int y = rand() % (gameMap.getHeight() - 2) + 2;
        Sprite ts(trashTextures[rand() % trashTextures.size()]);
        ts.setPosition(static_cast<float>(x * 70), static_cast<float>(y * 48));
        trashSprites.push_back(ts);
    }

    while (window.isOpen()) {
        float time = clock.getElapsedTime().asMicroseconds() / 800.0f;
        clock.restart();

        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) window.close();

            if (event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::R) {
                    tasksVisible = !tasksVisible;
                }

                if (event.key.code == Keyboard::E) {
                    for (auto& obj : gameObjects) {
                        if (obj->isNear(fox)) {
                            obj->interact(fox);
                            break;
                        }
                    }
                }
            }
        }

        Vector2f worldPos = window.mapPixelToCoords(Mouse::getPosition(window));
        if (ExitSprite.getGlobalBounds().contains(worldPos) && !keeper->dialogueOpened) {
            ExitSprite.setTexture(ExitLightTexture);
            if (Mouse::isButtonPressed(Mouse::Left)) {
                view.reset(FloatRect(0, 0, static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y)));
                window.setView(view);
                menu(window, music, isSoundOn);
            }
        }
        else {
            ExitSprite.setTexture(ExitTexture);
        }

        if (Keyboard::isKeyPressed(Keyboard::Space)) {
            if (fox.messageDisplayed) { fox.messageDisplayed = false; keeper->onAllTrashCollect(); }
            if (keeper->endTestShown) keeper->endTestShown = false;
        }

        if (Keyboard::isKeyPressed(Keyboard::A)) { fox.dir = 1; fox.speed = 0.5f; CurrentFrame += 0.005f * time; if (CurrentFrame > 3) CurrentFrame -= 3; fox.sprite.setTextureRect(IntRect(138 * int(CurrentFrame), 492, 138, 96)); }
        if (Keyboard::isKeyPressed(Keyboard::D)) { fox.dir = 0; fox.speed = 0.5f; CurrentFrame += 0.005f * time; if (CurrentFrame > 3) CurrentFrame -= 3; fox.sprite.setTextureRect(IntRect(128 * int(CurrentFrame), 192, 128, 96)); }
        if (Keyboard::isKeyPressed(Keyboard::W)) { fox.dir = 3; fox.speed = 0.5f; CurrentFrame += 0.005f * time; if (CurrentFrame > 3) CurrentFrame -= 3; fox.sprite.setTextureRect(IntRect(135 * int(CurrentFrame), 332, 135, 96)); }
        if (Keyboard::isKeyPressed(Keyboard::S)) { fox.dir = 2; fox.speed = 0.5f; CurrentFrame += 0.005f * time; if (CurrentFrame > 3) CurrentFrame -= 3; fox.sprite.setTextureRect(IntRect(143 * int(CurrentFrame), 61, 143, 96)); }

        getplayercoordinateforview(fox.getplayercoordinateX(), fox.getplayercoordinateY());
        fox.update(time, gameMap);
        fox.interactWithTrash(trashSprites, holeSprite);

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

        window.draw(holeSprite);
        for (auto& trash : trashSprites) window.draw(trash);

        window.draw(fox.sprite);
        for (auto& obj : gameObjects) {
            if (auto plant = dynamic_cast<Plants*>(obj.get())) {
                plant->drawInformWindow(window);
            }
        }

        View currentView = window.getView();
        CounterSprite.setPosition(currentView.getCenter().x + currentView.getSize().x / 2 - CounterSprite.getGlobalBounds().width, currentView.getCenter().y - currentView.getSize().y / 2);
        trashCounter.setPosition(view.getCenter().x + 313, view.getCenter().y - 243);
        ExitSprite.setPosition(currentView.getCenter().x - currentView.getSize().x / 2, currentView.getCenter().y + currentView.getSize().y / 2 - ExitSprite.getGlobalBounds().height);
        TaskSprite.setPosition(currentView.getCenter().x - currentView.getSize().x / 2, currentView.getCenter().y - currentView.getSize().y / 2);

        stringstream ss;
        ss << "Собрано мусора: " << fox.collectedTrash << " / " << numTrashSprites;
        trashCounter.setString(ss.str());

        window.draw(CounterSprite);
        window.draw(ExitSprite);
        window.draw(trashCounter);
        if (tasksVisible) window.draw(TaskSprite);

        float startOffsetY = -250.0f, stepY = 20.0f;
        int taskIndex = 0;
        for (const auto& item : taskInitList) {
            if (tasks.find(item.first) != tasks.end()) {
                tasks[item.first]->updatePosition(window, Vector2f(-460, startOffsetY + (taskIndex * stepY)));
                tasks[item.first]->draw(window);
            }
            taskIndex++;
        }

        if (fox.messageDisplayed) {
            message.setPosition(currentView.getCenter().x - message.getGlobalBounds().width / 2, currentView.getCenter().y - message.getGlobalBounds().height / 2);
            window.draw(message);
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
        keeper->drawKeeperDialogue(window, event, 50, 250);
        keeper->updateDialogue();

        bool allRed = true;
        for (auto const& pair : tasks) {
            if (pair.second->color != Color::Red) { allRed = false; break; }
        }
        if (allRed) {
            for (auto const& pair : tasks) pair.second->remove();
            keeper->onAllTasksCompleted();
        }

        if (keeper->endTestShown) {
            window.setView(keeper->uiView);
            window.draw(keeper->endTestSprite);
            window.setView(currentView);
        }

        window.display();
    }
    return 0;
}