// CGT 215 Final Project.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <vector>
#include <cmath>

const int WINDOW_WIDTH = 400;
const int WINDOW_HEIGHT = 400;
const float CELL_SIZE = 80.0f;
const int MAZE_SIZE = 5;

bool isPlaying = true;
int currentFace = 0;
sf::Vector2f gravity(0, 0.1f);

std::vector<sf::RectangleShape> cubeFaces[6];

sf::CircleShape player(20.0f);
sf::Vector2f playerVelocity(0.0f, 0.0f);
const float playerSpeed = 2.0f;

sf::CircleShape goal(15.0f);
sf::Vector2f goalPosition;

struct Obstacle {
    sf::CircleShape shape;
    sf::Vector2f velocity;
};

std::vector<Obstacle> obstacles;
const int NUM_OBSTACLES = 3;

int score = 0;

void setupCubeFaces() {
    for (int face = 0; face < 6; ++face) {
        for (int i = 0; i < MAZE_SIZE; ++i) {
            for (int j = 0; j < MAZE_SIZE; ++j) {
                sf::RectangleShape cell(sf::Vector2f(CELL_SIZE - 2, CELL_SIZE - 2));
                cell.setFillColor(sf::Color::White);
                cell.setOutlineColor(sf::Color::Black);
                cell.setOutlineThickness(1);
                cell.setPosition(j * CELL_SIZE, i * CELL_SIZE);
                cubeFaces[face].push_back(cell);
            }
        }
    }
}

void resetPlayerPosition() {
    switch (currentFace) {
    case 0:  // Top face
        player.setPosition(CELL_SIZE, CELL_SIZE);
        break;
    case 1:  // Bottom face
        player.setPosition(CELL_SIZE, CELL_SIZE);
        break;
    case 2:  // Left face
        player.setPosition(CELL_SIZE, CELL_SIZE);
        break;
    case 3:  // Right face
        player.setPosition(CELL_SIZE, CELL_SIZE);
        break;
    case 4:  // Front face
        player.setPosition(CELL_SIZE, CELL_SIZE);
        break;
    case 5:  // Back face
        player.setPosition(CELL_SIZE, CELL_SIZE);
        break;
    }
    playerVelocity = sf::Vector2f(0, 0);
}

void setGoalPosition() {
    int goalRow = rand() % MAZE_SIZE;
    int goalCol = rand() % MAZE_SIZE;

    goalPosition = sf::Vector2f(goalCol * CELL_SIZE + 5, goalRow * CELL_SIZE + 5);
    goal.setPosition(goalPosition);
}

void applyGravity() {
    if (playerVelocity.x == 0 && playerVelocity.y == 0) {
        if (currentFace == 0) gravity = sf::Vector2f(0, 0.1f);  // Face 0: Top face
        else if (currentFace == 1) gravity = sf::Vector2f(0, -0.1f);  // Face 1: Bottom face
        else if (currentFace == 2) gravity = sf::Vector2f(-0.1f, 0);  // Face 2: Left face
        else if (currentFace == 3) gravity = sf::Vector2f(0.1f, 0);   // Face 3: Right face
        else if (currentFace == 4) gravity = sf::Vector2f(0, 0.1f);  // Face 4: Front face
        else if (currentFace == 5) gravity = sf::Vector2f(0, -0.1f); // Face 5: Back face
    }

    playerVelocity += gravity;
    player.move(playerVelocity);
}

void handleInput() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        playerVelocity.x -= playerSpeed;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        playerVelocity.x += playerSpeed;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        playerVelocity.y -= playerSpeed;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        playerVelocity.y += playerSpeed;
    }
}

void moveObstacles() {
    for (auto& obstacle : obstacles) {
        obstacle.shape.move(obstacle.velocity);

        sf::Vector2f pos = obstacle.shape.getPosition();
        if (pos.x <= 0 || pos.x + obstacle.shape.getRadius() * 2 >= WINDOW_WIDTH) {
            obstacle.velocity.x = -obstacle.velocity.x;
        }
        if (pos.y <= 0 || pos.y + obstacle.shape.getRadius() * 2 >= WINDOW_HEIGHT) {
            obstacle.velocity.y = -obstacle.velocity.y;
        }
    }
}

void resetObstacles() {
    for (auto& obstacle : obstacles) {
        obstacle.shape.setPosition(rand() % (WINDOW_WIDTH - 30), rand() % (WINDOW_HEIGHT - 30));
        obstacle.velocity.x = (rand() % 3 - 1) * 2.0f;
        obstacle.velocity.y = (rand() % 3 - 1) * 2.0f;
    }
}

void handleCollision() {
    for (auto& cell : cubeFaces[currentFace]) {
        if (player.getGlobalBounds().intersects(cell.getGlobalBounds())) {
            playerVelocity = -playerVelocity * 0.5f;
        }
    }

    if (player.getGlobalBounds().intersects(goal.getGlobalBounds())) {
        score += 100;
        currentFace = (currentFace + 1) % 6;
        resetPlayerPosition();
        setGoalPosition();
        resetObstacles();  // Randomize obstacle positions on level change
    }

    for (auto& obstacle : obstacles) {
        if (player.getGlobalBounds().intersects(obstacle.shape.getGlobalBounds())) {
            score = 0;
            resetPlayerPosition();
            setGoalPosition();
            resetObstacles();  // Randomize obstacle positions on collision
        }
    }

    sf::Vector2f pos = player.getPosition();
    if (pos.x < 0 || pos.x + player.getRadius() * 2 > MAZE_SIZE * CELL_SIZE ||
        pos.y < 0 || pos.y + player.getRadius() * 2 > MAZE_SIZE * CELL_SIZE) {
        score = 0;
        currentFace = (currentFace + 1) % 6;
        resetPlayerPosition();
        setGoalPosition();
        resetObstacles();  // Randomize obstacle positions on level change
    }
}

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Gravity Cube Maze");
    window.setFramerateLimit(60);

    setupCubeFaces();
    resetPlayerPosition();
    setGoalPosition();

    for (int i = 0; i < NUM_OBSTACLES; ++i) {
        Obstacle obstacle;
        obstacle.shape.setRadius(15.0f);
        obstacle.shape.setFillColor(sf::Color::Yellow);
        obstacle.shape.setPosition(rand() % (WINDOW_WIDTH - 30), rand() % (WINDOW_HEIGHT - 30));

        obstacle.velocity.x = (rand() % 3 - 1) * 2.0f;
        obstacle.velocity.y = (rand() % 3 - 1) * 2.0f;

        obstacles.push_back(obstacle);
    }

    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Error loading font!" << std::endl;
        return -1;
    }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
                isPlaying = !isPlaying;
            }
        }

        if (isPlaying) {
            handleInput();
            applyGravity();
            handleCollision();
            moveObstacles();
        }

        window.clear(sf::Color::Blue);

        for (const auto& cell : cubeFaces[currentFace]) {
            window.draw(cell);
        }

        goal.setFillColor(sf::Color::Green);
        window.draw(goal);

        player.setFillColor(sf::Color::Red);
        window.draw(player);

        for (const auto& obstacle : obstacles) {
            window.draw(obstacle.shape);
        }

        sf::Text scoreText;
        scoreText.setFont(font);
        scoreText.setString("Score: " + std::to_string(score));
        scoreText.setCharacterSize(24);
        scoreText.setFillColor(sf::Color::Black);
        window.draw(scoreText);

        window.display();
    }
    return 0;
}







// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
