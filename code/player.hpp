#include <SFML/Graphics.hpp>
#include "constant.hpp"

class Player{
public:
    Player(int radius);
    sf::CircleShape shape;
    sf::RectangleShape gun;
    int rotationDirection;
    float angle;
    float yvelocity;
    float xvelocity;

    void setPosition(float x, float y);
};

