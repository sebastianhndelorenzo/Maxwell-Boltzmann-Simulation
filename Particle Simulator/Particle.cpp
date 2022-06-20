#include "Particle.h"

void Particle::initVariables() {
	//Set x velocity
	int randomNum = 2*((rand() % 2) - 0.5f);
	this->velocityVector.push_back(randomNum * 1103.840717f);
	//Set y velocity
	randomNum = 2 * ((rand() % 2) - 0.5f);
	this->velocityVector.push_back(randomNum * 1103.840717f);
	this->radius = 5.f;
	this->mass = this->radius*this->radius*3.142;
}

void Particle::initShape(const sf::RenderWindow& window, Box box) {
	this->shape.setRadius(this->radius);
	this->shape.setFillColor(sf::Color::Black);
	
	//Sets position inside box randomly
	int xrange = box.getShape().getGlobalBounds().width - this->shape.getGlobalBounds().width;
	int yrange = box.getShape().getGlobalBounds().height - this->shape.getGlobalBounds().height;
	this->shape.setPosition(sf::Vector2f(static_cast<float>(rand() % xrange) + box.getShape().getPosition().x, static_cast<float>(rand() % yrange) + box.getShape().getPosition().y));

	this->velocityIndicator.setSize(sf::Vector2f(5.f, 200.f));
	this->velocityIndicator.setFillColor(sf::Color::Blue);
}

Particle::Particle(const sf::RenderWindow& window, Box box) {
	this->initVariables();
	this->initShape(window, box);
}

Particle::~Particle() {
}

const sf::CircleShape Particle::getShape() const {
	return this->shape;
}

void Particle::checkBoxCollision(float secondsPerFrame, Box box) {
	//Checks if the particle has gone past the box bounds, and moves the particle back inside if it has
	//Top
	if (this->shape.getGlobalBounds().top <= box.getShape().getGlobalBounds().top) {
		this->velocityVector[1] = -this->velocityVector[1];
		float verticalOverpenetration = box.getShape().getPosition().y - this->shape.getPosition().y;
		this->shape.move(0, verticalOverpenetration);
	}
	//Bottom
	else if (this->shape.getGlobalBounds().top + this->shape.getGlobalBounds().height >= box.getShape().getGlobalBounds().top + box.getShape().getGlobalBounds().height) {
		this->velocityVector[1] = -this->velocityVector[1];
		float verticalOverpenetration = this->shape.getPosition().y - (box.getShape().getPosition().y + box.getShape().getGlobalBounds().height - this->shape.getGlobalBounds().height);
		this->shape.move(0, -verticalOverpenetration);
	}
	//Left
	if (this->shape.getGlobalBounds().left <= box.getShape().getGlobalBounds().left) {
		this->velocityVector[0] = -this->velocityVector[0];
		float horizontalOverpenetration = box.getShape().getPosition().x - this->shape.getPosition().x;
		this->shape.move(horizontalOverpenetration, 0);
	}
	//Right
	else if (this->shape.getGlobalBounds().left + this->shape.getGlobalBounds().width >= box.getShape().getGlobalBounds().left + box.getShape().getGlobalBounds().width) {
		this->velocityVector[0] = -this->velocityVector[0];
		float horizontalOverpenetration = this->shape.getPosition().x - (box.getShape().getPosition().x + box.getShape().getGlobalBounds().width - this->shape.getGlobalBounds().width);
		this->shape.move(-horizontalOverpenetration, 0);
	}
}

void Particle::velUpdate(float secondsPerFrame, Box box) {

}

void Particle::posUpdate(float secondsPerFrame, Box box) {
	this->velUpdate(secondsPerFrame, box);
	if (paused == false) {
		this->shape.move(this->velocityVector[0] * secondsPerFrame, -this->velocityVector[1] * secondsPerFrame);
	}
	this->checkBoxCollision(secondsPerFrame, box);
}

void Particle::update(float secondsPerFrame, Box box) {
	this->posUpdate(secondsPerFrame, box);
	this->velocityIndicator.setPosition(sf::Vector2f(this->shape.getPosition().x + 0.5*this->shape.getGlobalBounds().width, this->shape.getPosition().y + 0.5 * this->shape.getGlobalBounds().height));
	this->velocityIndicator.setRotation(atan(this->velocityVector[1] / this->velocityVector[0]) * 180/3.142);
	if (this->velocityVector[1] > 0) {
		this->velocityIndicator.setRotation(atan(this->velocityVector[1] / this->velocityVector[0]) * 180 / 3.142 + 180);
	}
}

void Particle::render(sf::RenderTarget* target) {
	target->draw(this->shape);
}
