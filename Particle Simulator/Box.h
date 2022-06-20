#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

class Box {
private:
	sf::RectangleShape outside;
	sf::RectangleShape inside;
	void initShape();
public:
	//Constructors/Destructors
	Box();
	virtual ~Box();

	//Functions
	const sf::RectangleShape& getShape() const;
	void update();
	void render(sf::RenderTarget* target);
};

