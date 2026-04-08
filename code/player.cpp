#include "player.hpp"

Player:: Player(int radius):shape(radius), gun(sf::Vector2f(1.5 * radius, 0.5 * radius)),rotationDirection(1), angle(0), yvelocity(0), xvelocity(0) {
    this->gun.setOrigin(0, 0.25 * radius);

}

void Player::setPosition(float x, float y){
    shape.setPosition(x, y);
    gun.setPosition(x + playerSize, y + playerSize);
}
