#include "screen.cpp"

int main()
{   
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "SFML works!");
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    window.setPosition(sf::Vector2i(
        (desktop.width - windowWidth) / 2,
        (desktop.height - windowHeight) / 2
    ));
    window.setActive();
    sf::Texture::getMaximumSize();
    
    gameState state = GAME;
    
    //font test
    sf::Font font;
    font.loadFromFile("textures/font.ttf");
    sf::Text scoreBoard;
    scoreBoard.setFont(font);
    scoreBoard.setString("TEST TEST");
    scoreBoard.setOutlineColor(sf::Color::Black);
    scoreBoard.setOutlineThickness(2);
    scoreBoard.setFillColor(sf::Color(255,215,80));
    scoreBoard.setCharacterSize(charSize);


    //textureLoader
    textureLoader textures;
                    
    //background initiailzation
    sf::Sprite background;
    background.setTexture(textures.background);
    background.setScale(
        (float)windowWidth / textures.background.getSize().x * 2,
        (float)floorHeight / textures.background.getSize().y * 1.01
    ); 

    //player inititalization
    Player player(playerSize);
    player.setPosition(windowWidth * 0.2,windowHeight * 0.6);
    player.shape.setTexture(&textures.ball);
    player.gun.setTexture(&textures.barrel);    

    //floor initialization
    float scalar = std::max((float) windowWidth / textures.floor.getSize().x,(float) (windowHeight - floorHeight) / textures.floor.getSize().y);
    sf::Sprite floor1;
    floor1.scale(scalar, scalar);
    floor1.setTexture(textures.floor);
    floor1.setPosition(0, floorHeight);
    
    sf::Sprite floor2;
    floor2.scale(scalar, scalar);
    floor2.setTexture(textures.floor);
    floor2.setPosition(floor2.getTexture()->getSize().x * scalar, floorHeight);
    

    //enemies initialization
    Queue enemies(10, ObstacleType::GROUND);
    enemies.push(textures.obstacles[0].getSize().x * groundRatio,
                 textures.obstacles[0].getSize().y * groundRatio,
                 textures, 0);

    //test
    Queue flyingEnemies(5, ObstacleType::FLIGHT);
    flyingEnemies.push(textures.flyingObstacles[0][0].getSize().x * airRatio,
                       textures.flyingObstacles[0][0].getSize().y * airRatio,
                       textures,0);

    float randomSpawnGround = 2.f;
    float randomSpawnFlight = 3.f;

    float score = 0;

    sf::Clock enemySpawnerGround;
    sf::Clock enemySpawnerFlight;
    sf::Clock frameClock;
    sf::Clock scoreClock;
    sf::Clock pauseClock;
    sf::Clock animationClock;
    window.setFramerateLimit(60);
    while (window.isOpen())
    {
        window.clear();

        window.draw(background);
        window.draw(floor1);
        window.draw(floor2);

        window.draw(player.gun);
        window.draw(player.shape);
        
        enemyDraw(enemies, window);
        enemyDraw(flyingEnemies, window);
        
       
        if(state == GAME) score += 10 * scoreClock.getElapsedTime().asSeconds(); 
        scoreClock.restart();
        scoreBoard.setString(std::to_string((int) score));
        scoreBoard.setPosition(windowWidth - scoreBoard.getLocalBounds().width, 0);
        window.draw(scoreBoard);

        switch(state){
            case GAME:
                gameLoop(window, state, textures, player, enemies, flyingEnemies, floor1, floor2, randomSpawnGround, randomSpawnFlight, background,
                         enemySpawnerGround, enemySpawnerFlight, frameClock, scoreClock, pauseClock, animationClock);
                         break;
            case MENU:
                menu(window, state, font, pauseClock, randomSpawnGround, randomSpawnFlight);
                break;
            case LOSESCREEN:
                bool restart = false;
                lose(window, state, font, score, restart);
                if(restart) restartGame(state, player, textures, enemies, flyingEnemies, enemySpawnerGround, enemySpawnerFlight, scoreClock, pauseClock, score);
                break;
        }
        frameClock.restart();
        window.display();
    } 
}