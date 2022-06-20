#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <string>

class Button {
private:
	std::vector<sf::Text> textList;
public:
	Button(int x_pos, int y_pos, int xSize, int ySize, std::string String);
	~Button();
	std::string String;
	sf::Font font;
	sf::Text text;
	sf::RectangleShape shape;
	void initFont();
	void initText();

	void render(sf::RenderTarget* target);
};

