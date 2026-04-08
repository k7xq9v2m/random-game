#include <SFML/Graphics.hpp>
#include <iostream>
#include "player.cpp"
#include "obstacles.cpp"
#include "physics.cpp"

enum gameState{
    GAME,
    MENU,
    LOSESCREEN,
};

void gameLoop(sf::RenderWindow& window,gameState& state ,textureLoader& textures, Player& player, Queue& enemies, Queue& flyingEnemies, sf::Sprite& floor1, sf::Sprite& floor2, float& randomSpawnGround, float& randomSpawnFlight, sf::Sprite& background,
               sf::Clock& enemySpawnerGround, sf::Clock& enemySpawnerFlight, sf::Clock& frameClock, sf::Clock& scoreClock, sf::Clock& pauseClock, sf::Clock& animationClock){
    sf::Event event;
    while (window.pollEvent(event))
    {
        if(event.type == sf::Event::Closed)
            window.close();
        if(event.type == sf::Event::KeyReleased){
            if(event.key.code == sf::Keyboard::Space){
                jump(player);
                player.rotationDirection *= -1;
            }
        }
        if(event.type == sf::Event::KeyPressed){
            if(event.key.code == sf::Keyboard::Escape){
                state = MENU;
                pauseClock.restart();
            }
        }       
    }

    float frameDelta = frameClock.getElapsedTime().asSeconds();

    floor1.move(-obstacleSpeed * frameClock.getElapsedTime().asSeconds(), 0);
    floor2.move(-obstacleSpeed * frameClock.getElapsedTime().asSeconds(), 0);
    if(floor1.getTexture()->getSize().x * floor1.getScale().x + floor1.getPosition().x < 0){
        floor1.setPosition(floor1.getTexture()->getSize().x * floor1.getScale().x + floor2.getPosition().x , floorHeight);
    }
    if(floor2.getTexture()->getSize().x * floor2.getScale().x + floor2.getPosition().x < 0){
        floor2.setPosition(floor2.getTexture()->getSize().x * floor2.getScale().x + floor1.getPosition().x , floorHeight);
    }

    if(background.getTexture()->getSize().x * background.getScale().x + background.getPosition().x < windowWidth) background.setPosition(0,0);
        background.move(frameDelta * - 10, 0);

    //gun rotation:: WICHTIG
    player.angle = player.gun.getRotation();
    player.gun.setRotation(player.angle + rotationSpeed * player.rotationDirection * frameDelta);

    //JUMPING DO NOT TOUCH
    player.shape.move(player.xvelocity  * frameDelta, player.yvelocity  * frameDelta);
    player.gun.move(player.xvelocity  * frameDelta, player.yvelocity  * frameDelta);
    if(player.shape.getGlobalBounds().intersects(floor1.getGlobalBounds()) || player.shape.getGlobalBounds().intersects(floor2.getGlobalBounds())){
        player.setPosition(player.shape.getPosition().x, floorHeight - 2 * playerSize);
        player.yvelocity *= -0.6f;
        player.xvelocity *= 0.7f;
        if(std::abs(player.yvelocity) < .05f){
            player.yvelocity = 0;
        }  
    }

    //Window border  ision
    if(player.shape.getPosition().y < 0){
        player.setPosition(player.shape.getPosition().x, 0);
        player.yvelocity *= -1.f;
    }
    if(player.shape.getPosition().x < 0){
        player.setPosition(0, player.shape.getPosition().y);
        player.xvelocity *= -0.4f;
    }
    if(player.shape.getPosition().x > windowWidth - playerSize * 2){
        player.setPosition(windowWidth - 2 * playerSize , player.shape.getPosition().y);
        player.xvelocity *= -0.4f;
    }
    yVelocityUpdate(player.yvelocity, frameDelta);
    xVelocityUpdate(player.xvelocity, frameDelta);

    //enemy spawn DO NOT TOUCH THE FORMULA
    float lastSpawned = enemySpawnerGround.getElapsedTime().asSeconds();
    bool spawned = enemySpawn(enemies, lastSpawned, randomSpawnGround, textures);
    if(spawned){
        enemySpawnerGround.restart();
        randomSpawnGround = ((float)(rand() % (int)(maxSpawnTime * 100)) + minSpawnTime * 100) / 100;
    }
    lastSpawned = enemySpawnerFlight.getElapsedTime().asSeconds();
    spawned = enemySpawn(flyingEnemies, lastSpawned, randomSpawnFlight, textures);
    if(spawned){
        enemySpawnerFlight.restart();
        randomSpawnFlight = ((float)(rand() % (int)(maxSpawnTimeFlying * 100)) + minSpawnTimeFlying * 100) / 100;
    }

    enemyMovement(enemies, frameDelta);
    enemyMovement(flyingEnemies, frameDelta);
    
    enemyDelete(enemies);
    enemyDelete(flyingEnemies);

    if(enemyCollision(player.shape, enemies)) state = LOSESCREEN;
    if(enemyCollision(player.shape, flyingEnemies))state = LOSESCREEN;

    if(animationClock.getElapsedTime().asSeconds() > aFps){
        int index = flyingEnemies.head;
        while(index != flyingEnemies.tail){
            flyingEnemies.container[index]->updateTexture(textures);
            index++;
            if(index >= flyingEnemies.length)
                index = 0;
        }
        animationClock.restart();
    }
}

    void menu(sf::RenderWindow& window, gameState& state, sf::Font& font, sf::Clock& pauseClock, float& randomSpawnGround, float& randomSpawnFlight){
        sf::Text continueText;
        continueText.setFont(font);
        continueText.setString("continue");
        continueText.setFillColor(sf::Color(200, 210, 220));
        continueText.setOutlineColor(sf::Color::Black);
        continueText.setOutlineThickness(1);
        continueText.setCharacterSize(charSize);
        continueText.setOrigin(continueText.getLocalBounds().width / 2,0);
        continueText.setPosition(sf::Vector2f(windowWidth / 2, windowHeight / 3));

        sf::Text quitText;
        quitText.setFont(font);
        quitText.setString("quit");
        quitText.setFillColor(sf::Color(200, 210, 220));
        quitText.setOutlineColor(sf::Color::Black);
        quitText.setOutlineThickness(1);
        quitText.setCharacterSize(charSize);
        quitText.setOrigin(quitText.getLocalBounds().width / 2.f,0);
        quitText.setPosition(sf::Vector2f(windowWidth / 2, windowHeight * 1/2));

        sf::Event event;
        while (window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
                window.close();
            if(event.type == sf::Event::KeyPressed){
                if(event.key.code == sf::Keyboard::Escape){
                    state = GAME;
                    float pausedTime = pauseClock.getElapsedTime().asSeconds();
                    randomSpawnGround += pausedTime;
                    randomSpawnFlight += pausedTime; 
                }
            }  
            if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
                if(continueText.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)){
                    state = GAME;
                    float pausedTime = pauseClock.getElapsedTime().asSeconds();
                    randomSpawnGround += pausedTime;
                    randomSpawnFlight += pausedTime;
                }
                if(quitText.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)){
                    window.close();
                }
            }     
        }
        window.draw(continueText);
        window.draw(quitText);
    }

    void lose(sf::RenderWindow& window,gameState& state, sf::Font& font,  int score, bool &restart){
        sf::Text scoreText;
        scoreText.setFont(font);
        scoreText.setString("Your score : " + std::to_string(score));
        scoreText.setFillColor(sf::Color(200, 210, 220));
        scoreText.setOutlineColor(sf::Color::Black);
        scoreText.setOutlineThickness(1);
        scoreText.setCharacterSize(42);
        scoreText.setCharacterSize(charSize);
        scoreText.setOrigin(scoreText.getLocalBounds().width / 2,0);
        scoreText.setPosition(sf::Vector2f(windowWidth / 2, windowHeight / 3));
                
        sf::Text quitText;
        quitText.setFont(font);
        quitText.setString("Press again (Space)/ Quit (Esc)");
        quitText.setFillColor(sf::Color(200, 210, 220));
        quitText.setOutlineColor(sf::Color::Black);
        quitText.setOutlineThickness(1);
        quitText.setCharacterSize(42);
        quitText.setCharacterSize(charSize);
        quitText.setOrigin(quitText.getLocalBounds().width / 2,0);
        quitText.setPosition(sf::Vector2f(windowWidth / 2, windowHeight * 1/2));

        sf::Event event;
        while (window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
                window.close();
            if(event.type == sf::Event::KeyReleased){  
                if(event.key.code == sf::Keyboard::Escape){
                    window.close();
                }
                if(event.key.code == sf::Keyboard::Space){
                    restart = true;
                }
            }   
        }
        window.draw(scoreText);
        window.draw(quitText);
    }

    void restartGame(gameState& state, Player& player, textureLoader& textures, Queue& enemies, Queue& flyingEnemies, sf::Clock& enemySpawnerGround, sf::Clock& enemySpawnerFlight, sf::Clock& scoreClock, sf::Clock& pauseClock, float& score){
        state = GAME;
        player.setPosition(100,350);
        player.xvelocity = 0;
        player.yvelocity = 0;
        player.angle = 0;
        player.rotationDirection = 1;
        player.gun.setRotation(0);

        for(int i = enemies.head; i != enemies.tail;){
            delete enemies.container[i];
            enemies.container[i] = nullptr;
            i++;
            if(i >= enemies.length) i = 0;
        }
        enemies.container.resize(enemies.length, nullptr);
        enemies.head = 0; 
        enemies.tail = 0;
        enemies.push(textures.obstacles[0].getSize().x * groundRatio,
                     textures.obstacles[0].getSize().y * groundRatio,
                     textures,
                     0);
        if(flyingEnemies.head == flyingEnemies.tail) std::cout << "fuck" << std::endl;
        for(int i = flyingEnemies.head; i != flyingEnemies.tail;){
            delete flyingEnemies.container[i];
            flyingEnemies.container[i] = nullptr;
            i++;
            if(i >= flyingEnemies.length) i = 0;
        }
        flyingEnemies.container.resize(flyingEnemies.length, nullptr);
        flyingEnemies.head = 0; 
        flyingEnemies.tail = 0;
        flyingEnemies.push(textures.flyingObstacles[0][0].getSize().x * airRatio,
                           textures.flyingObstacles[0][0].getSize().y * airRatio,
                           textures,
                           0);

        enemySpawnerGround.restart();
        enemySpawnerFlight.restart();
        scoreClock.restart();
        pauseClock.restart();

        score = 0;

    }




















