#include <SFML/Graphics.hpp>
#include <iostream>
#include "textureLoader.cpp"
#include "constant.hpp"

enum ObstacleType{
    GROUND = 1,
    FLIGHT = 2
};

class Obstacle{
    public:
    Obstacle();
    Obstacle(int width, int height, textureLoader& texture, int type);
    Obstacle(const Obstacle& obstacle);
    virtual void updateTexture(textureLoader& texture){};
    sf::Texture* texture;
    sf::Sprite sprite;
    sf::RectangleShape hitbox;
}; 


class flyingObstacle: public Obstacle{
    public:
    flyingObstacle();
    flyingObstacle(int height, int width, float yPosition, textureLoader& texture, int birdType);
    flyingObstacle(const flyingObstacle& other);
    void updateTexture(textureLoader& texture) override;
    int frame;
    int birdType;
};


class Queue{
    public:
    int length;
    int head;
    int tail;
    bool isFull;
    ObstacleType type;
    std::vector<Obstacle*> container;

    Queue(int length, ObstacleType type);
    void pop();
    void push(int height, int width, textureLoader& texture, int type);
    bool isEmpty();
};
//PS should have just make a loop function that takes some other function
void enemyMovement(Queue& enemies, float frameDelta);

bool enemySpawn(Queue& enemies, float lastSpawned,  float spawnRate, textureLoader& textures);

void enemyDelete(Queue& enemies);

bool enemyCollision(sf::CircleShape& player, Queue& enemies);

void enemyDraw(Queue& enemies, sf::Window& window);

