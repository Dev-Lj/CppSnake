#include "Game.h"
#include <SFML/Graphics.hpp>
#include "Snake.h"
#include <vector>
#include <ctime>

// Temporary for debug purposes
#include <iostream>
#include <string>

sf::Vector2f getRandomApplePos(int xTiles, int yTiles, float tileSize) {
    std::srand(std::time(nullptr));
    return sf::Vector2f((rand() % xTiles) * tileSize, ((rand() % yTiles) * tileSize)+50);
}

void Game::startGameLoop() {
    int xTiles = 30;
    int yTiles = 30;
    float tileSize = 20;
    float tickRate = 0.25;
    bool waitTick = false;
    int points = 0;

    sf::RenderWindow window(sf::VideoMode(xTiles * tileSize, (yTiles * tileSize)+50), "Snake");

    // Grid block
    sf::RectangleShape block(sf::Vector2f(tileSize, tileSize));
    sf::Color yellowGreen(0xbce784ff);
    block.setFillColor(yellowGreen);
    block.setOutlineThickness(1.f);
    block.setOutlineColor(sf::Color::Black);

    // Snake block
    sf::Texture snakeHeadTexture;
    snakeHeadTexture.loadFromFile("Sprites\\snake_head.png");
    sf::Texture snakeBodyTexture;
    snakeBodyTexture.loadFromFile("Sprites\\snake_body.png");
    sf::Texture snakeTailTexture;
    snakeTailTexture.loadFromFile("Sprites\\snake_tail.png");
    sf::Texture snakeTurnTexture;
    snakeTurnTexture.loadFromFile("Sprites\\snake_turn.png");

    sf::Sprite snakeBlock;
    snakeBlock.setOrigin(sf::Vector2f(tileSize / 2, tileSize / 2));
    Snake snake(xTiles/2, yTiles-(Snake::INITIAL_LENGTH + 1), xTiles, yTiles);

    // Apple block
    sf::Texture appleTexture;
    if (!appleTexture.loadFromFile("Sprites\\apple.png"))
        std::cout << "Failed to load Apple Texture" << std::endl;
    appleTexture.setSmooth(true);
    sf::Sprite appleBlock;
    appleBlock.setTexture(appleTexture);
    appleBlock.setPosition(getRandomApplePos(xTiles, yTiles, tileSize));

    // Font for Points display
    sf::Font font;
    if (!font.loadFromFile("arial.ttf"))
        std::cout << "Failed to load font." << std::endl;
    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(20);
    scoreText.setStyle(sf::Text::Bold);
    scoreText.setString(std::to_string(points) + " X ");
    scoreText.setPosition(sf::Vector2f(((xTiles * tileSize) / 2) - ((scoreText.getGlobalBounds().width + appleBlock.getGlobalBounds().width) / 2), 15.f));
    sf::Sprite demoApple = appleBlock;
    demoApple.setPosition(sf::Vector2f(scoreText.getGlobalBounds().left + scoreText.getGlobalBounds().width, 17.f));
    

    // Init clock, basic for tickrate
    sf::Clock clock;

    // Gameloop
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::KeyPressed && !waitTick) {
                waitTick = true;
                switch (event.key.code) {
                case sf::Keyboard::Right:
                    snake.turnRight();
                    break;
                case sf::Keyboard::Left:
                    snake.turnLeft();
                    break;
                }
            }
        }

        if (!snake.getIsAlive()) {
            window.close();
        }

        if (clock.getElapsedTime().asSeconds() >= tickRate) {
            snake.move();
            clock.restart();
            waitTick = false;
        }
        

        window.clear();

        // Draw all tiles

        for (int x = 0; x < xTiles; x++)
            for (int y = 0; y < yTiles; y++) {
                block.setPosition(sf::Vector2f(x * tileSize, (y * tileSize)+50));
                window.draw(block);
            }

        std::vector<snakepart> snakeParts = snake.getSnakeParts();
        for (int i = 0;i < snakeParts.size(); i++) {
            snakeBlock.setScale(sf::Vector2f(1, 1));
            switch (snakeParts[i].snakePartType)
            {
            case Snake::SNAKEPART_HEAD:
                snakeBlock.setTexture(snakeHeadTexture);
                break;
            case Snake::SNAKEPART_BODY:
                snakeBlock.setTexture(snakeBodyTexture);
                break;
            case Snake::SNAKEPART_TAIL:
                snakeBlock.setTexture(snakeTailTexture);
                break;
            case Snake::SNAKEPART_TURNRIGHT:
                snakeBlock.setTexture(snakeTurnTexture);
                break;
            case Snake::SNAKEPART_TURNLEFT:
                snakeBlock.setTexture(snakeTurnTexture);
                snakeBlock.setScale(sf::Vector2f(-1, 1));
                break;
            }
            snakeBlock.setPosition(sf::Vector2f((snakeParts[i].x * tileSize) + tileSize/2, (snakeParts[i].y * tileSize)+50 + tileSize / 2));
            snakeBlock.setRotation(snakeParts[i].direction * 90);
            window.draw(snakeBlock);
        }

        if (snakeParts[0].x * tileSize == appleBlock.getPosition().x && (snakeParts[0].y * tileSize) + 50 == appleBlock.getPosition().y) {
            snake.grow();
            points++;
            scoreText.setString(std::to_string(points) + " X ");
            appleBlock.setPosition(getRandomApplePos(xTiles, yTiles, tileSize));
        }

        window.draw(appleBlock);
        window.draw(scoreText);
        window.draw(demoApple);

        window.display();
        
    }
}