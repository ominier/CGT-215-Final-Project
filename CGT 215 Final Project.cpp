// CGT 215 Final Project.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <vector>
#include <cmath>

// Constants
const int WINDOW_WIDTH = 400;
const int WINDOW_HEIGHT = 400;
const float CELL_SIZE = 80.0f;  // Increase cell size to fit better
const int MAZE_SIZE = 5;  // 5x5 grid per face

// Game Variables
bool isPlaying = true;
int currentFace = 0; // Tracks the current face of the cube (0 to 5 for each face)
sf::Vector2f gravity(0, 0.1f);  // Simulated gravity on the current face

// Define the faces of the cube
std::vector<sf::RectangleShape> cubeFaces[6];

// Define player
sf::CircleShape player(20.0f); // Increased radius for the player
sf::Vector2f playerVelocity(0.0f, 0.0f);
const float playerSpeed = 2.0f; // Player speed constant to control movement speed

// Goal marker (position will change for each face)
sf::CircleShape goal(15.0f); // Goal is represented by a green circle
sf::Vector2f goalPosition; // Position of the goal on the current face

// Score system
int score = 0;

// Setup cube faces with cells
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

// Reset player position based on the current face
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

// Function to randomize the goal position on the current face
void setGoalPosition() {
    // Generate random row and column for the goal
    int goalRow = rand() % MAZE_SIZE;  // Random row between 0 and MAZE_SIZE-1
    int goalCol = rand() % MAZE_SIZE;  // Random column between 0 and MAZE_SIZE-1

    // Calculate the position based on the cell size
    goalPosition = sf::Vector2f(goalCol * CELL_SIZE + 5, goalRow * CELL_SIZE + 5);  // Add a small offset to center the goal in the cell

    goal.setPosition(goalPosition);  // Set the goal's position
}

// Apply gravity based on the current face
void applyGravity() {
    // Only apply gravity when the player is not moving
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

// Handle player input (movement)
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

// Handle collisions with the maze walls and switch faces accordingly
void handleCollision() {
    // Collision detection against cells of the current face
    for (auto& cell : cubeFaces[currentFace]) {
        if (player.getGlobalBounds().intersects(cell.getGlobalBounds())) {
            playerVelocity = -playerVelocity * 0.5f; // Basic bounce with damping
        }
    }

    // Check if the player hits the goal
    if (player.getGlobalBounds().intersects(goal.getGlobalBounds())) {
        // Player reached the goal
        score += 100;  // Award points for reaching the goal
        currentFace = (currentFace + 1) % 6; // Move to the next face (cycle through 6 faces)
        resetPlayerPosition(); // Reset the player's position
        setGoalPosition(); // Set a new goal position for the new face
    }

    // Check if the player hits the edges of the current face and switch faces accordingly
    sf::Vector2f pos = player.getPosition();
    if (pos.x < 0) {
        currentFace = (currentFace + 2) % 6;  // Move to an adjacent face
        resetPlayerPosition();
        setGoalPosition();  // Update goal position
    }
    if (pos.x + player.getRadius() * 2 > MAZE_SIZE * CELL_SIZE) {
        currentFace = (currentFace + 1) % 6;  // Move to an adjacent face
        resetPlayerPosition();
        setGoalPosition();  // Update goal position
    }
    if (pos.y < 0) {
        currentFace = (currentFace + 4) % 6;  // Move to an adjacent face
        resetPlayerPosition();
        setGoalPosition();  // Update goal position
    }
    if (pos.y + player.getRadius() * 2 > MAZE_SIZE * CELL_SIZE) {
        currentFace = (currentFace + 5) % 6;  // Move to an adjacent face
        resetPlayerPosition();
        setGoalPosition();  // Update goal position
    }
}




// Main game loop
int main() {
    // Create the window
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Gravity Cube Maze");
    window.setFramerateLimit(60);

    setupCubeFaces();
    resetPlayerPosition();
    setGoalPosition();

    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Error loading font!" << std::endl;
        return -1;
    }

    // Main rendering loop
    // Main rendering loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
                isPlaying = !isPlaying; // Toggle play/pause
            }
        }

        if (isPlaying) {
            handleInput();
            applyGravity();
            handleCollision();
        }

        // Rendering
        window.clear(sf::Color::Blue);

        // Draw the current face's cells
        for (const auto& cell : cubeFaces[currentFace]) {
            window.draw(cell);
        }

        // Draw the goal
        goal.setFillColor(sf::Color::Green);
        window.draw(goal);

        // Draw the player
        player.setFillColor(sf::Color::Red);
        window.draw(player);

        // Display the score
        sf::Font font;
        if (font.loadFromFile("arial.ttf")) {
            sf::Text scoreText;
            scoreText.setFont(font);
            scoreText.setString("Score: " + std::to_string(score));
            scoreText.setCharacterSize(24);
            scoreText.setFillColor(sf::Color::Black);
            window.draw(scoreText);
        }

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
