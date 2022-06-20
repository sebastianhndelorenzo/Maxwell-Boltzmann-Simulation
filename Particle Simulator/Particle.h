#pragma once
#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

#include "Box.h"

class Particle {
private:
	void initVariables();
	void initShape(const sf::RenderWindow& window, Box box);

	//Variables
public:
	//Constructors/Destructors
	Particle(const sf::RenderWindow& window, Box box);
	virtual ~Particle();

	//Variables
	sf::CircleShape shape;
	std::vector<float> velocityVector;
	sf::RectangleShape velocityIndicator;
	float radius;
	float mass;
	bool paused = false;

	//Functions
	const sf::CircleShape getShape() const;
	void checkBoxCollision(float secondsPerFrame, Box box);
	void velUpdate(float secondsPerFrame, Box box);
	void posUpdate(float secondsPerFrame, Box box);
	void update(float secondsPerFrame, Box box);
	void render(sf::RenderTarget* target);
};

