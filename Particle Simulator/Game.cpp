#include "Game.h"

void Game::initVariables() {
	this->secondsPerFrame = 0.001f;
	this->numParticles = 200;
	this->paused = false;
	this->step = 0;

	//Graph
	for (int i = 0; i < graph.numberOfBars; i++) {
		this->roundedVelocities.push_back({int(pow((i+1)*(graph.xMax-graph.xMin)/graph.numberOfBars, 2)), 0});
	}
}

void Game::initButtons() {
	//Resets averageBalls
	Button resetAverageBalls(1380, 930, 300, 50, "Reset average indicators");
	this->buttons.push_back(resetAverageBalls);

	//Temperature controls
	int xStart = 150;
	int yStart = 930;
	Button decreaseTemperatureby100(xStart, yStart, 100, 50, "-100K");
	this->buttons.push_back(decreaseTemperatureby100);
	Button decreaseTemperatureby10(xStart + 130, yStart, 100, 50, "-10K");
	this->buttons.push_back(decreaseTemperatureby10);
	Button decreaseTemperatureby1(xStart + 2 * 130, yStart, 100, 50, "-1K");
	this->buttons.push_back(decreaseTemperatureby1);
	Button increaseTemperatureby1(xStart + 3 * 130, yStart, 100, 50, "+1K");
	this->buttons.push_back(increaseTemperatureby1);
	Button increaseTemperatureby10(xStart + 4 * 130, yStart, 100, 50, "+10K");
	this->buttons.push_back(increaseTemperatureby10);
	Button increaseTemperatureby100(xStart + 5 * 130, yStart, 100, 50, "+100K");
	this->buttons.push_back(increaseTemperatureby100);

	//Pause button
	Button pauseButton(1000, yStart, 150, 50, "Pause/Play");
	buttons.push_back(pauseButton);
}

void Game::initWindow() {
	this->videoMode = sf::VideoMode(2000, 1300);
	this->window = new sf::RenderWindow(this->videoMode, "Particle Simulator", sf::Style::Close | sf::Style::Titlebar);
	this->window->setFramerateLimit(600);
}

void Game::initParticles() {
	for (int i = 0; i < numParticles; i++) {
		this->particles.push_back(Particle(*this->window, this->box));
	}
	particles[0].shape.setRadius(10.f);
	particles[0].shape.setFillColor(sf::Color::Red);
}

Game::Game() {
	this->initButtons();
	this->initVariables();
	this->initWindow();
	this->initParticles();
	this->initFonts();
	this->initText();
}

Game::~Game() {
	delete this->window;
}

void Game::initFonts() {
	this->font.loadFromFile("Fonts/design.graffiti.comicsansmsgras.ttf");
}

void Game::initText() {
	this->text.setFont(this->font);
	this->text.setCharacterSize(30);
	this->text.setPosition(0.f, 0.f);
	this->text.setFillColor(sf::Color::Black);
}

void Game::pollEvents() {
	while (this->window->pollEvent(this->sfmlEvent)) {
		switch (this->sfmlEvent.type) {
		case sf::Event::Closed:
			this->window->close();
			break;
		case sf::Event::MouseButtonPressed:
			//BUTTON LOGIC
			for (int i = 0; i < buttons.size(); i++) {
				if (sf::Mouse::getPosition(*this->window).x >= buttons[i].shape.getPosition().x
					&& sf::Mouse::getPosition(*this->window).x <= buttons[i].shape.getPosition().x + buttons[i].shape.getGlobalBounds().width
					&& sf::Mouse::getPosition(*this->window).y >= buttons[i].shape.getPosition().y
					&& sf::Mouse::getPosition(*this->window).y <= buttons[i].shape.getPosition().y + buttons[i].shape.getGlobalBounds().height) {
					switch (i) {
					case 0:
						//Resets averageBalls
						for (int i = 0; i < graph.averageDensities.size(); i++) {
							graph.averageDensities[i] = 0;
						}
						graph.resetStep = this->step;
						break;
					//Temperature controls
					case 1:
						if (graph.temperature > 99) {
							this->decreaseTemperature(100);
							this->updateGraphBars();
							this->updateAverageBalls();
							this->updateText();
						}
						break;
					case 2:
						if (graph.temperature > 9) {
							this->decreaseTemperature(10);
							this->updateGraphBars();
							this->updateAverageBalls();
							this->updateText();
						}
						break;
					case 3:
						if (graph.temperature > 1) {
							this->decreaseTemperature(1);
							this->updateGraphBars();
							this->updateAverageBalls();
							this->updateText();
						}
						break;
					case 4:
						this->increaseTemperature(1);
						this->updateGraphBars();
						this->updateAverageBalls();
						this->updateText();
						break;
					case 5: 
						this->increaseTemperature(10);
						this->updateGraphBars();
						this->updateAverageBalls();
						this->updateText();
						break;
					case 6:
						this->increaseTemperature(100);
						this->updateGraphBars();
						this->updateAverageBalls();
						this->updateText();
						break;
					case 7:
						if (this->paused == false) {
							this->paused = true;
						}
						else {
							this->paused = false;
						}
						break;
					}
				}
			}
			break;
		case sf::Event::KeyPressed:
			if (this->sfmlEvent.key.code == sf::Keyboard::Escape) {
				this->window->close();
				break;
			}
		default:
			break;
		}
	}
}

void Game::updateText() {
	std::stringstream string;
	/*float sum = 0;
	for (int i = 0; i < particles.size(); i++) {
		sum += pow(float(particles[i].velocityVector[0]), 2) + pow(float(particles[i].velocityVector[1]), 2);
	}*/
	string << "Step: " << this->step << " Temperature: " << graph.temperature << "K";
	this->text.setString(string.str());
}

void Game::update() {
	this->pollEvents();
	if (this->paused == false) {
		for (int i = 0; i < particles.size(); i++) {
			particles[i].update(this->secondsPerFrame, this->box);
		}
		this->updateText();

		//Naive collision checking
		/**
		for (int i = 0; i < particles.size() - 1; i++) {
			for (int j = i + 1; j < particles.size(); j++) {
				this->checkParticleOnParticleCollision(i, j);
			}
		}*/

		//Broad phase - kdTree

		//Broad phase - Sweep and prune along x-axis
		this->sortParticles();
		for (int i = 0; i < particles.size(); i++) {
			activeVector.push_back(i);
			if (i == particles.size() - 1
				|| particles[i].shape.getPosition().x + particles[i].shape.getGlobalBounds().width < particles[i+1].shape.getPosition().x) {
				//Narrow phase
				for (int j = 0; j < activeVector.size() - 1; j++) {
					for (int k = j + 1; k < activeVector.size(); k++) {
						this->checkParticleOnParticleCollision(activeVector[j], activeVector[k]);
					}
				}
				activeVector = {};
			}
		}

		//Collecting data on rounded velocities for graph
		for (int i = 0; i < roundedVelocities.size(); i++) {
			roundedVelocities[i][1] = 0;
		}

		for (Particle i : particles) {
			for (int j = 0; j < roundedVelocities.size(); j++) {
				if (roundedVelocities[j][0] >= pow(i.velocityVector[0], 2) + pow(i.velocityVector[1], 2)) {
					roundedVelocities[j][1] += 1;
					break;
				}
			}
		}

		this->graphUpdate();
		this->step += 1;
	}
}

void Game::sortParticles() {
	//Sorts the particles along a given dimension using insertion sort
	for (int i = 1; i < particles.size(); i++) {
		int currentID = i;
		for (int j = i; j > 0; j--) {
			if (particles[currentID].shape.getPosition().x < particles[currentID - 1].shape.getPosition().x) {
				std::swap(particles[currentID], particles[currentID - 1]);
				currentID -= 1;
			}
			else {
				break;
			}
		}
	}
}

void Game::updateGraphBars() {
	for (int i = 0; i < roundedVelocities.size(); i++) {
		graph.currentDensities[i] = float(roundedVelocities[i][1]) / numParticles / ((graph.xMax - graph.xMin) / graph.numberOfBars) * graph.ds;
		graph.bars[i].setSize(sf::Vector2f(graph.bars[i].getGlobalBounds().width, -graph.currentDensities[i] / graph.dy));
	}
}

void Game::updateAverageBalls() {
	//Updates average frequency density indicators
	for (int i = 0; i < graph.averageDensities.size(); i++) {
		graph.averageDensities[i] = (graph.averageDensities[i] * (this->step - graph.resetStep) + graph.currentDensities[i]) / (this->step - graph.resetStep + 1);
		graph.averageBalls[i].setPosition(graph.averageBalls[i].getPosition().x, graph.yBaseline - graph.averageDensities[i] / graph.dy);
	}
}
void Game::graphUpdate() {
	this->updateGraphBars();
	this->updateAverageBalls();
}

std::vector<float> Game::scalarTimesVector(float scalar, std::vector<float> vector) {
	std::vector<float> returnVector = { scalar * vector[0], scalar * vector[1]};
	return returnVector;
}

float Game::magnitude(std::vector<float> vector) {
	return sqrt(pow(vector[0], 2) + pow(vector[1], 2));
}

std::vector<float> Game::subtractVectors(std::vector<float> vector1, std::vector<float> vector2) {
	//Subtracts vector2 from vector1
	std::vector<float> returnVector = {vector1[0] - vector2[0], vector1[1] - vector2[1]};
	return returnVector;
}

std::vector<float> Game::normalize(std::vector<float> vector) {
	float magnitude = this->magnitude(vector);
	std::vector<float> returnVector = {vector[0]/magnitude, vector[1]/magnitude};
	return returnVector;
}

void Game::render() {
	this->window->clear(sf::Color{ 252, 249, 242 });
	this->box.render(this->window);
	this->graph.render(this->window);
	for (Particle i: particles) {
		i.render(this->window);
	}
	for (Button i : buttons) {
		i.render(this->window);
	}
	this->renderText(*this->window);
	this->window->display();
}

void Game::decreaseTemperature(int deltaT) {
	this->graph.temperature -= deltaT;
	this->graph.updateGraphParams();

	float sum = 0;
	float targetSum = 2 * this->numParticles * graph.boltzConstant * graph.temperature / graph.mass;
	for (int i = 0; i < particles.size(); i++) {
		sum += pow(float(particles[i].velocityVector[0]), 2) + pow(float(particles[i].velocityVector[1]), 2);
	}
	for (int i = 0; i < particles.size(); i++) {
		if (sum > targetSum) {
			sum -= pow(particles[i].velocityVector[0], 2) + pow(particles[i].velocityVector[1], 2);
			particles[i].velocityVector[0] = 0;
			particles[i].velocityVector[1] = 0;
		}
		else if (sum < targetSum) {
			float difference = targetSum - sum;
			float newVelocity = sqrt(pow(particles[i].velocityVector[0], 2) + pow(particles[i].velocityVector[1], 2) + difference);
			particles[i].velocityVector[0] = newVelocity / sqrt(2);
			particles[i].velocityVector[1] = newVelocity / sqrt(2);
			break;
		}
	}
}

void Game::increaseTemperature(int deltaT) {
	graph.temperature += deltaT;
	graph.updateGraphParams();
	float sum = 0;
	float targetSum = 2 * this->numParticles * graph.boltzConstant * graph.temperature / graph.mass;
	for (int i = 0; i < particles.size(); i++) {
		sum += pow(float(particles[i].velocityVector[0]), 2) + pow(float(particles[i].velocityVector[1]), 2);
	}
	float difference = targetSum - sum;
	float newVelocity = sqrt(pow(particles[0].velocityVector[0], 2) + pow(particles[0].velocityVector[1], 2) + difference);
	particles[0].velocityVector[0] = newVelocity / sqrt(2);
	particles[0].velocityVector[1] = newVelocity / sqrt(2);
}

//Checks if particles collide and changes velocities
void Game::checkParticleOnParticleCollision(int i, int j){
	//Gives you the horizontal and vertical separations between the centres of the circles
	float horizontalSeparation = particles[i].shape.getPosition().x + 0.5 * particles[i].shape.getGlobalBounds().width - particles[j].shape.getPosition().x - 0.5 * particles[j].shape.getGlobalBounds().width;
	float verticalSeparation = particles[i].shape.getPosition().y + 0.5 * particles[i].shape.getGlobalBounds().height - particles[j].shape.getPosition().y - 0.5 * particles[j].shape.getGlobalBounds().height;
	float touchingCentreToCentreDistance = 0.5 * (particles[i].getShape().getGlobalBounds().width + particles[j].getShape().getGlobalBounds().width);
	if (pow(horizontalSeparation, 2) + pow(verticalSeparation, 2) <= pow(touchingCentreToCentreDistance, 2)) {
		std::vector<float> leftPosition = {particles[i].shape.getPosition().x + 0.5f * particles[i].shape.getGlobalBounds().width, this->window->getSize().y - particles[i].shape.getPosition().y - 0.5f * particles[i].shape.getGlobalBounds().height};
		std::vector<float> rightPosition = { particles[j].shape.getPosition().x + 0.5f * particles[j].shape.getGlobalBounds().width, this->window->getSize().y - particles[j].shape.getPosition().y - 0.5f * particles[j].shape.getGlobalBounds().height};

		//Prevents particles phasing into each other
		float overlap = 0.5 * (particles[i].shape.getGlobalBounds().width + particles[j].shape.getGlobalBounds().width) - sqrt(verticalSeparation * verticalSeparation + horizontalSeparation * horizontalSeparation);
		std::vector<float> positionDifference = subtractVectors(rightPosition, leftPosition);
		std::vector<float> normalizedPositionDifference = normalize(positionDifference);
		particles[j].shape.move(scalarTimesVector(0.5 * overlap, normalizedPositionDifference)[0], scalarTimesVector(-0.5 * overlap, normalizedPositionDifference)[1]);
		particles[i].shape.move(scalarTimesVector(-0.5 * overlap, normalizedPositionDifference)[0], scalarTimesVector(0.5 * overlap, normalizedPositionDifference)[1]);

		std::vector<float> tempVector = subtractVectors(particles[i].velocityVector, particles[j].velocityVector);
		std::vector<float> velocityDifference = subtractVectors(particles[j].velocityVector, particles[i].velocityVector);
		particles[j].velocityVector = subtractVectors(particles[j].velocityVector, scalarTimesVector(2 * particles[i].mass / (particles[i].mass + particles[j].mass) * dotProduct(velocityDifference, positionDifference) / pow(magnitude(positionDifference), 2), positionDifference));

		velocityDifference = tempVector;
		positionDifference = subtractVectors(leftPosition, rightPosition);
		particles[i].velocityVector = subtractVectors(particles[i].velocityVector, scalarTimesVector(2 * particles[j].mass / (particles[i].mass + particles[j].mass) * dotProduct(velocityDifference, positionDifference) / pow(magnitude(positionDifference), 2), positionDifference));
	}
}

std::vector<float> Game::addVectors(std::vector<float> vector1, std::vector<float> vector2) {
	std::vector<float> returnVector = { vector1[0] + vector2[0], vector1[1] + vector2[1] };
	return returnVector;
}

void Game::renderText(sf::RenderTarget& target) {
	target.draw(this->text);
}

const bool Game::running() const {
	return this->window->isOpen();
}

float Game::dotProduct(std::vector<float> vector1, std::vector<float> vector2) {
	float sum = 0;
	for (int i = 0; i < vector1.size(); i++) {
		sum += vector1[i] * vector2[i];
	}
	return sum;
}