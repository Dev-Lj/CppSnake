#include "Game.h"
#include <SFML/Graphics.hpp>
#include "Snake.h"
#include <vector>
#include <ctime>
#include <iostream>

// Temporary for debug purposes
#include <iostream>
#include <string>

const int Game::AMOUNT_TILES_X = 30;
const int Game::AMOUNT_TILES_Y = 30;
const float Game::SIZE_PER_TILE = 20;
const float Game::TICK_TIMEOUT_SECONDS = 0.25;


sf::Vector2f getRandomApplePos(int xTiles, int yTiles, float tileSize) {
    std::srand(std::time(nullptr));
    return sf::Vector2f((rand() % xTiles) * tileSize, ((rand() % yTiles) * tileSize)+50);
}

sf::RectangleShape initGridBlock(float sizePerTile) {
    sf::RectangleShape block(sf::Vector2f(sizePerTile, sizePerTile));
    sf::Color yellowGreen(0xbce784ff);
    block.setFillColor(yellowGreen);
    block.setOutlineThickness(1.f);
    block.setOutlineColor(sf::Color::Black);
    return block;
}

void Game::startGameLoop() {
    bool waitTick = false;
    int points = 0;

    sf::RenderWindow window(sf::VideoMode(Game::AMOUNT_TILES_X * SIZE_PER_TILE, (AMOUNT_TILES_Y * SIZE_PER_TILE)+50), "Snake");

    // Grid block
    sf::RectangleShape block = initGridBlock(SIZE_PER_TILE);

    // Load snake sprites
    sf::Texture snakeHeadTexture;
    snakeHeadTexture.loadFromFile("sprites\\snake_head.png");
    sf::Texture snakeBodyTexture;
    snakeBodyTexture.loadFromFile("sprites\\snake_body.png");
    sf::Texture snakeTailTexture;
    snakeTailTexture.loadFromFile("sprites\\snake_tail.png");
    sf::Texture snakeTurnTexture;
    snakeTurnTexture.loadFromFile("sprites\\snake_turn.png");

    sf::Sprite snakeBlock;
    snakeBlock.setOrigin(sf::Vector2f(SIZE_PER_TILE / 2, SIZE_PER_TILE / 2));
    Snake snake(AMOUNT_TILES_X/2, AMOUNT_TILES_Y-(Snake::INITIAL_LENGTH + 1), AMOUNT_TILES_X, AMOUNT_TILES_Y);

    // Apple block
    sf::Texture appleTexture;
    if (!appleTexture.loadFromFile("sprites\\apple.png"))
        std::cout << "Failed to load Apple Texture" << std::endl;
    appleTexture.setSmooth(true);
    sf::Sprite appleBlock;
    appleBlock.setTexture(appleTexture);
    appleBlock.setPosition(getRandomApplePos(AMOUNT_TILES_X, AMOUNT_TILES_Y, SIZE_PER_TILE));

    // Font for Points display
    sf::Font font;
    if (!font.loadFromFile("arial.ttf"))
        std::cout << "Failed to load font." << std::endl;
    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(20);
    scoreText.setStyle(sf::Text::Bold);
    scoreText.setString(std::to_string(points) + " X ");
    scoreText.setPosition(sf::Vector2f(((AMOUNT_TILES_X * SIZE_PER_TILE) / 2) - ((scoreText.getGlobalBounds().width + appleBlock.getGlobalBounds().width) / 2), 15.f));
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

        if (!snake.isAlive()) {
            window.close();
        }

        if (clock.getElapsedTime().asSeconds() >= TICK_TIMEOUT_SECONDS) {
            snake.move();
            clock.restart();
            waitTick = false;
        }
        

        window.clear();

        // Draw all tiles

        for (int x = 0; x < AMOUNT_TILES_X; x++)
            for (int y = 0; y < AMOUNT_TILES_Y; y++) {
                block.setPosition(sf::Vector2f(x * SIZE_PER_TILE, (y * SIZE_PER_TILE)+50));
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
            snakeBlock.setPosition(sf::Vector2f((snakeParts[i].x * SIZE_PER_TILE) + SIZE_PER_TILE/2, (snakeParts[i].y * SIZE_PER_TILE)+50 + SIZE_PER_TILE / 2));
            snakeBlock.setRotation(snakeParts[i].direction * 90);
            window.draw(snakeBlock);
        }

        if (snakeParts[0].x * SIZE_PER_TILE == appleBlock.getPosition().x && (snakeParts[0].y * SIZE_PER_TILE) + 50 == appleBlock.getPosition().y) {
            snake.grow();
            points++;
            scoreText.setString(std::to_string(points) + " X ");
            appleBlock.setPosition(getRandomApplePos(AMOUNT_TILES_X, AMOUNT_TILES_Y, SIZE_PER_TILE));
        }

        window.draw(appleBlock);
        window.draw(scoreText);
        window.draw(demoApple);

        window.display();
        
    }
}