#include "Box.h"

void Box::initShape() {
	this->outside.setPosition(30, 40);
	this->outside.setFillColor(sf::Color::Black);
	this->outside.setSize(sf::Vector2f(1000.f, 800.f));

	this->inside.setPosition(50, 60);
	this->inside.setFillColor(sf::Color{sf::Color{ 252, 249, 242 }});
	this->inside.setSize(sf::Vector2f(960.f, 760.f));
}

Box::Box() {
	this->initShape();
}

Box::~Box() {
}

const sf::RectangleShape& Box::getShape() const {
	return this->inside;
}

void Box::update() {
}

void Box::render(sf::RenderTarget* target) {
	target->draw(this->outside);
	target->draw(this->inside);	
}
