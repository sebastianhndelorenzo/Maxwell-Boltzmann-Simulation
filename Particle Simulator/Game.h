#pragma once
#include <iostream>
#include <ctime>
#include <vector>
#include <math.h>
#include <sstream>
#include <array>

#include "Particle.h"
#include "Box.h"
#include "Graph.h"
#include "Button.h"

class Game {
private:
	//Setup
	sf::VideoMode videoMode;
	sf::RenderWindow* window;
	sf::Event sfmlEvent;

	//Variables
	float secondsPerFrame;
	std::vector<Particle> particles;
	std::vector<Button> buttons;
	std::vector<int> activeVector;
	int numParticles;
	Box box;
	Graph graph;
	std::vector<std::array<int, 2>> roundedVelocities;
	sf::Font font;
	sf::Text text;
	bool paused;
	int step;
	
	//Initialisation
	void initVariables();
	void initWindow();
	void initParticles();
	void initFonts();
	void initText();
	void initButtons();
	
public:
	//Constructors/Destructors
	Game();
	virtual ~Game();

	//Accessors
	const bool running() const;


	//Functions
	void sortParticles();
	void checkParticleOnParticleCollision(int i, int j);
	void pollEvents();
	void updateText();
	void update();
	void updateGraphBars();
	void updateAverageBalls();
	void graphUpdate();
	void decreaseTemperature(int deltaT);
	void increaseTemperature(int deltaT);

	void renderIndicators();
	void renderText(sf::RenderTarget& target);
	void render();

	//Vector operations
	float dotProduct(std::vector<float> vector1, std::vector<float> vector2);
	std::vector<float> scalarTimesVector(float scalar, std::vector<float> vector);
	std::vector<float> addVectors(std::vector<float> vector1, std::vector<float> vector2);
	std::vector<float> subtractVectors(std::vector<float> vector1, std::vector<float> vector2);
	std::vector<float> normalize(std::vector<float> vector);
	float magnitude(std::vector<float> vector);
};

