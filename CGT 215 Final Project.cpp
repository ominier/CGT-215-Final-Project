// CGT 215 Final Project.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <vector>
#include <cmath>

//Constants
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const float CELL_SIZE = 40.0f;
const int MAZE_SIZE = 5;  // 5x5 grid per face

// Game Variables
bool isPlaying = true;
int currentFace = 0; // Tracks the current face of the cube (0 to 5 for each face)
sf::Vector2f gravity(0, 0.1f);  // Simulated gravity on the current face

// Define the faces of the cube
std::vector<sf::RectangleShape> cubeFaces[6];

// Define player
sf::CircleShape player(15.0f);
sf::Vector2f playerVelocity(0.0f, 0.0f);

void setupCubeFaces() {
	for (int face = 0; face < 6; ++face) {
		for (int i = 0; i < MAZE_SIZE; ++i) {
			for (int j = 0; j < MAZE_SIZE; ++j) {
				sf::RectangleShape cell(sf:: Vector2f(CELL_SIZE - 2, CELL_SIZE - 2));
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
	// Position the player at the start of the current face
	player.setPosition(CELL_SIZE, CELL_SIZE);
	playerVelocity = sf::Vector2f(0, 0);
}

void applyGravity() {
	playerVelocity += gravity;
	player.move(playerVelocity);
}

void switchFace(int newFace) {
	currentFace = newFace;
	resetPlayerPosition();
}

void handleCollision() {
	for (auto& cell : cubeFaces[currentFace]) {
		if (player.getGlobalBounds().intersects(cell.getGlobalBounds())) {
			// Basic collision resolution (bounce effect)
			playerVelocity = -playerVelocity * 0.5f; // Basic bounce with damping
		}
	}

	// Check if the player hits the edges and switch faces accordingly
	sf::Vector2f pos = player.getPosition();
	if (pos.x < 0) {
		switchFace((currentFace + 5) % 6); // Wrap to the left face
	}
	if (pos.x + player.getRadius() * 2 > WINDOW_WIDTH) {
		switchFace((currentFace + 1) % 6); // Wrap to the right face
	}
	if (pos.y < 0) {
		switchFace((currentFace + 4) % 6); // Wrap to the top face
	}
	if (pos.y + player.getRadius() * 2 > WINDOW_HEIGHT) {
		switchFace((currentFace + 2) % 6); // Wrap to the bottom face
	}
}

void handleInput() {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		playerVelocity.x -= 0.1f;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		playerVelocity.x += 0.1f;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		playerVelocity.y -= 0.1f;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		playerVelocity.y += 0.1f;
	}
}

int main() {
	// Create the window
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Gravity Cube Maze");
	window.setFramerateLimit(60);

	setupCubeFaces();
	resetPlayerPosition();

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

		// Draw the current face
		for (const auto& cell : cubeFaces[currentFace]) {
			window.draw(cell);
		}

		// Draw the player
		player.setFillColor(sf::Color::Red);
		window.draw(player);

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
