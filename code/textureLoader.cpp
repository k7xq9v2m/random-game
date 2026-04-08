#include <SFML/Graphics.hpp>
#include <iostream>

struct hitBox{
    sf::Vector2f offset;
    sf::Vector2f size;
    hitBox(){};
    hitBox(sf::Vector2f offset, sf::Vector2f size){
        this->size = size;
        this->offset = offset;
    }
};

class textureLoader{
    public:
    sf::Texture floor;
    sf::Texture background;
    sf::Texture ball;
    sf::Texture barrel;
    std::vector<sf::Texture> obstacles;
    std::vector<std::vector<sf::Texture>> flyingObstacles;
    std::map<int, int> map;
    std::map<int,hitBox> groundHitbox;
    std::map<int,hitBox> flightHitbox;


    textureLoader(){
        background.loadFromFile("textures/himmel.png"); 
        floor.loadFromFile("textures/groundtest.png");
        ball.loadFromFile("textures/ball.png");
        barrel.loadFromFile("textures/barrel.png");


        obstacles.resize(8);
        obstacles[0].loadFromFile("textures/16x32 trees.png", sf::Rect(0,0,16,32));
        obstacles[1].loadFromFile("textures/16x32 trees.png", sf::Rect(16,0,16,32));
        obstacles[2].loadFromFile("textures/16x32 trees.png", sf::Rect(32,0,16,32));
        obstacles[3].loadFromFile("textures/16x32 trees.png", sf::Rect(48,0,16,32));
        obstacles[4].loadFromFile("textures/16x32 trees.png", sf::Rect(0,32,16,32));
        obstacles[5].loadFromFile("textures/16x32 trees.png", sf::Rect(16,32,16,32));
        obstacles[6].loadFromFile("textures/16x32 trees.png", sf::Rect(32,32,16,32));
        obstacles[7].loadFromFile("textures/16x32 trees.png", sf::Rect(48,32,16,32));
        groundHitbox.insert({{0,hitBox(sf::Vector2f(4,7), sf::Vector2f(8,24))}, {1,hitBox(sf::Vector2f(0,12), sf::Vector2f(16,20))}, {2,hitBox(sf::Vector2f(6,11), sf::Vector2f(5,21))},
                             {3,hitBox(sf::Vector2f(5,17), sf::Vector2f(11,15))}, {4,hitBox(sf::Vector2f(3,3), sf::Vector2f(11,29))}, {5,hitBox(sf::Vector2f(4,2), sf::Vector2f(8,30))},
                             {6,hitBox(sf::Vector2f(0,8), sf::Vector2f(16,24))}, {7,hitBox(sf::Vector2f(6,13), sf::Vector2f(10,19))}});


        flyingObstacles.resize(4, std::vector<sf::Texture>(3));
        flyingObstacles[0][0].loadFromFile("textures/bird4.png", sf::Rect(0,0,32,32));
        flyingObstacles[0][1].loadFromFile("textures/bird4.png", sf::Rect(32,0,32,32));
        flyingObstacles[0][2].loadFromFile("textures/bird4.png", sf::Rect(64,0,32,32));

        flyingObstacles[1][0].loadFromFile("textures/bird2.png", sf::Rect(0,0,32,32));
        flyingObstacles[1][1].loadFromFile("textures/bird2.png", sf::Rect(32,0,32,32));
        flyingObstacles[1][2].loadFromFile("textures/bird2.png", sf::Rect(64,0,32,32)); 
        
        flyingObstacles[2][0].loadFromFile("textures/bird3.png", sf::Rect(0,0,32,32));
        flyingObstacles[2][1].loadFromFile("textures/bird3.png", sf::Rect(32,0,32,32));
        flyingObstacles[2][2].loadFromFile("textures/bird3.png", sf::Rect(64,0,32,32));  
        flightHitbox.insert({{0,hitBox(sf::Vector2f(2,10), sf::Vector2f(14,7))}, {1,hitBox(sf::Vector2f(2,9), sf::Vector2f(18,13))}, {2,hitBox(sf::Vector2f(2,9), sf::Vector2f(18,13))}});



        map.insert({{1,8}, {2,3}});
    }
};