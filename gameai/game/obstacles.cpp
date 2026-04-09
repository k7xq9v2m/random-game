#include "obstacles.hpp"

    Obstacle::Obstacle(){};
    Obstacle:: Obstacle(int width, int height,  textureLoader& texture, int type): sprite(), texture(){
        sf::Vector2f scalar = sf::Vector2f((float)width / texture.obstacles[type].getSize().x,(float)height / texture.obstacles[type].getSize().y);
        sprite.setTexture(texture.obstacles[type]);
        sprite.setScale(scalar.x, scalar.y);
        sprite.setPosition(windowWidth, floorHeight - height + 5); //+5 for trees to be from the ground

        hitbox = sf::RectangleShape(sf::Vector2f(scalar.x * texture.groundHitbox[type].size.x , scalar.y * texture.groundHitbox[type].size.y )); 
        hitbox.setPosition(sf::Vector2f(texture.groundHitbox[type].offset.x * scalar.x + sprite.getPosition().x , texture.groundHitbox[type].offset.y * scalar.y + sprite.getPosition().y)); 

    };
    Obstacle:: Obstacle(const Obstacle& other){
        this->texture = other.texture;
        this->sprite = other.sprite;
        this->sprite.setTexture(*this->texture); 
        this->hitbox = other.hitbox;
    };    

    flyingObstacle::flyingObstacle(){};
    flyingObstacle::flyingObstacle(int width, int height, float yPosition,  textureLoader& texture, int obstacleType): birdType(obstacleType){
        sf::Vector2f scalar = sf::Vector2f((float)width / texture.flyingObstacles[obstacleType][0].getSize().x,(float)height / texture.flyingObstacles[obstacleType][0].getSize().y);    
        sprite.setTexture(texture.flyingObstacles[obstacleType][0]);
        sprite.setScale((float)width / texture.flyingObstacles[obstacleType][0].getSize().x,(float)height / texture.flyingObstacles[obstacleType][0].getSize().y);
        sprite.setPosition(windowWidth, yPosition); 

        hitbox = sf::RectangleShape(sf::Vector2f(scalar.x * texture.flightHitbox[obstacleType].size.x , scalar.y * texture.flightHitbox[obstacleType].size.y)); 
        hitbox.setPosition(sf::Vector2f(texture.flightHitbox[obstacleType].offset.x * scalar.x + sprite.getPosition().x , texture.flightHitbox[obstacleType].offset.y * scalar.y + sprite.getPosition().y)); 
        frame = 0;
    };
    flyingObstacle:: flyingObstacle(const flyingObstacle& other) : Obstacle(other){
        this->frame = other.frame;
        this->birdType = other.birdType;
    };

    void flyingObstacle::updateTexture(textureLoader& texture){
        frame++;
        if(frame >= 3) frame = 0;
        sprite.setTexture(texture.flyingObstacles[birdType][frame]);
    };


    Queue::Queue(int length, ObstacleType type):length(length), type(type), head(0), tail(0), isFull(false){
        container.resize(length, nullptr);
    };
    void Queue::pop(){
        delete container[head];
        container[head] = nullptr;
        head++;
        if(head >= length) head = 0;
        isFull = false;
    };

    void Queue::push(int width, int height,  textureLoader& texture, int obstacleType){  
        if(isFull == false){
            switch(type){
                case GROUND:
                    container[tail] = new Obstacle(width, height, texture, obstacleType);
                    break;
                case FLIGHT:
                    container[tail] = new flyingObstacle(width, height, rand() %  (floorHeight - 2 * (windowHeight - floorHeight)), texture, obstacleType);
            }
            tail++;
            if(tail >= length) tail = 0;
            if(tail == head) isFull = true;
        }
        else{
            std::cout << "too many enemies" << std::endl;
        }

    };

    bool Queue::isEmpty(){
        return !isFull and (tail == head);
    };

    void enemyMovement(Queue& enemies, float frameDelta){
        float speed = 0;
        if(enemies.type == GROUND) speed = obstacleSpeed;
        if(enemies.type == FLIGHT) speed = obstacleSpeedFlight;
        int index = enemies.head;
        if(!enemies.isFull){
            while(index != enemies.tail){
                enemies.container[index]->sprite.move(-speed * frameDelta, 0);
                enemies.container[index]->hitbox.move(-speed * frameDelta, 0);
                index++;
                if(index >= enemies.length)
                    index = 0;
            }
        }
    }

    bool enemySpawn(Queue& enemies, float lastSpawned, float spawnRate, textureLoader& textures){
        if(lastSpawned > spawnRate){
            int random = rand() % textures.map[enemies.type];
            switch(enemies.type){
                case GROUND:
                    enemies.push(textures.obstacles[random].getSize().x * groundRatio,
                    textures.obstacles[random].getSize().y * groundRatio,
                    textures,
                    random);
                    break;
                case FLIGHT:
                    enemies.push(textures.flyingObstacles[random][0].getSize().x * airRatio,
                    textures.flyingObstacles[random][0].getSize().y * airRatio,
                    textures,
                    random);
            }
            return true;
        }
        return false;
    }

    void enemyDelete(Queue& enemies){
        while(enemies.container[enemies.head] != nullptr && enemies.head != enemies.tail){
            if(enemies.container[enemies.head]->sprite.getPosition().x + (enemies.container[enemies.head]->sprite.getScale().x * enemies.container[enemies.head]->sprite.getTexture()->getSize().x) <= 0){
                enemies.pop();
            }
            else{
                break;
            }
        }
    }

    bool enemyCollision(sf::CircleShape& player, Queue& enemies){
        int index = enemies.head;
        if(!enemies.isFull){
            while(index != enemies.tail){
                if(player.getGlobalBounds().intersects(enemies.container[index]->hitbox.getGlobalBounds())){
                    return true;
                }
                index++;
                if(index >= enemies.length)
                    index = 0;
            }
        }
        return false;
    }

    void enemyDraw(Queue& enemies, sf::RenderWindow& window){
        int index = enemies.head;
        while(index != enemies.tail){
            window.draw(enemies.container[index]->sprite);
            //window.draw(enemies.container[index]->hitbox);
            index++;
            if(index >= enemies.length)
                index = 0;
        }
    }
