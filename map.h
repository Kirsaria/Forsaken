#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <map>

using namespace sf;
using namespace std;

class Map {
private:
    int heightMap;
    int widthMap;
    String* tileMap;
    Texture mapTexture;
    Sprite mapSprite;
    map<char, IntRect> tileRects;

public:
    Map() {
        heightMap = 25;
        widthMap = 40;

        tileMap = new String[heightMap]{
            "0--------------------------------------=",
            "0   g           g     q         g      =",
            "0                     g    r           =",
            "0  t   q    t      q          g        =",
            "0     g        r                  t    =",
            "0                          t           =",
            "0          t     t g            t      =",
            "0    t                     g           =",
            "0    t   r               t     r       =",
            "0                  r               q   =",
            "0  q          t  t                     =",
            "0        g              g  r    r      =",
            "0                          g           =",
            "0    r    q   g   1222223       g   g  =",
            "0                 4555556  t           =",
            "0      t      r   4555556       r      =",
            "0  g    r     r   7888889         r    =",
            "0         r         r      g           =",
            "0             g     q             g    =",
            "0    g                    q    r     t =",
            "0         t          g   t             =",
            "0   g        t                 g       =",
            "0                 r       r            =",
            "0    r     g q      t  g       r       =",
            "0--------------------------------------="
        };

        mapTexture.loadFromFile("Image/map4.png");
        mapSprite.setTexture(mapTexture);

        tileRects = {
            {' ', IntRect(70, 0, 135, 48)}, {'-', IntRect(70, 0, 135, 48)},
            {'0', IntRect(0, 0, 80, 48)}, {'=', IntRect(1135, 0, 1027, 48)},
            {'1', IntRect(397, 0, 466, 48)}, {'2', IntRect(466, 0, 535, 48)},
            {'3', IntRect(499, 0, 570, 48)}, {'4', IntRect(570, 0, 638, 48)},
            {'5', IntRect(638, 0, 706, 48)}, {'6', IntRect(706, 0, 777, 48)},
            {'7', IntRect(778, 0, 848, 48)}, {'8', IntRect(809, 0, 877, 48)},
            {'9', IntRect(933, 0, 1001, 48)}, {'g', IntRect(196, 0, 264, 48)},
            {'r', IntRect(135, 0, 203, 48)}, {'t', IntRect(1490, 0, 1558, 48)},
            {'q', IntRect(328, 0, 396, 48)}
        };
    }

    ~Map() {
        delete[] tileMap;
    }

    int getHeight() const { return heightMap; }
    int getWidth() const { return widthMap; }

    bool isCollisionTile(int i, int j) const {
        if (i < 0 || i >= heightMap || j < 0 || j >= widthMap) return false;
        char tile = tileMap[i][j];
        return (tile == '0' || tile == '-' || tile == '=' || tile == '7' || tile == '8' || tile == '9');
    }

    void draw(RenderWindow& window) {
        for (int i = 0; i < heightMap; i++) {
            for (int j = 0; j < widthMap; j++) {
                char tile = tileMap[i][j];
                auto it = tileRects.find(tile);
                if (it != tileRects.end()) {
                    mapSprite.setTextureRect(it->second);
                    mapSprite.setPosition(j * 70, i * 48);
                    window.draw(mapSprite);
                }
            }
        }
    }
};