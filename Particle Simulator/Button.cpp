#include "Button.h"

void Button::initFont() {
	font.loadFromFile("Fonts/design.graffiti.comicsansmsgras.ttf");
}

void Button::render(sf::RenderTarget* target) {
	target->draw(this->shape);
	this->initText();
	target->draw(this->text);
}


Button::Button(int x_pos, int y_pos, int xSize, int ySize, std::string String) {
	this->shape.setPosition(x_pos, y_pos);
	this->shape.setSize(sf::Vector2f(xSize, ySize));
	this->shape.setFillColor(sf::Color{ 240, 164, 132 });
	this->initFont();
	this->String = String;
	this->initText();
}

void Button::initText() {
	this->text.setFont(this->font);
	this->text.setString(this->String);
	this->text.setCharacterSize(20.f);
	this->text.setFillColor(sf::Color::Black);
	this->text.setPosition(this->shape.getPosition().x + 0.5 * this->shape.getGlobalBounds().width - 0.5 * this->text.getGlobalBounds().width, this->shape.getPosition().y + 12);
}

Button::~Button() {
}
