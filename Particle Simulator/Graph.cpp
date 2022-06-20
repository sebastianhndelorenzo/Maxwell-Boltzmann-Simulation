#include "Graph.h"

void Graph::initVariables() {
	this->mass = 3.32 * pow(10, -27);
	this->boltzConstant = 1.380649f * pow(10, -23);
	this->temperature = 293;

	this->xMin = 0;
	this->xMax = 4000;

	this->initAxes();
	//Change in velocity per pixel along the x-axis
	this->ds = (xMax - xMin) / (xAxis.getGlobalBounds().width - yAxis.getGlobalBounds().width);

	this->numberOfBars = 20;
	this->xNumberOfNumbers = 8;
	this->yNumberOfNumbers = 10;
	//Ensures that yMax is always higher than the mode velocity's frequency density
	this->yMax = 1.2f * mass / boltzConstant / temperature * sqrt(boltzConstant * temperature / mass) * exp(-0.5) * ds;
	//Change in frequencity density per pixel along the y-axis
	this->dy = (yMax - yMin) / (yAxis.getGlobalBounds().height - xAxis.getGlobalBounds().height);
	//this->yMax = 0.003;
}

void Graph::updateAxes() {
	for (int i = 0; i < yTextList.size(); i++) {
		yTextList[i].setString(std::to_string(yMin + i * (yMax - yMin) / (yNumberOfNumbers - 1)));
	}
}

void Graph::redrawBoltzPlot() {
	if (this->temperature > 0) {
		for (int i = 0; i < boltzmannPlot.size(); i++) {
			boltzmannPlot[i].setPosition(xAxis.getPosition().x + yAxis.getGlobalBounds().width, xAxis.getPosition().y - 0.5 * boltzmannPlot[0].getGlobalBounds().height);
			//Incorporate proportionality to the y axis to the code below
			boltzmannPlot[i].move(i, -mass * i * ds / (boltzConstant * temperature) * exp(-mass * pow(i * ds, 2) / (2 * boltzConstant * temperature)) * ds / dy);
		}
	}
}

void Graph::updateGraphParams() {
	//Ensures that yMax is always higher than the mode velocity's frequency density
	this->yMax = 1.2f * mass / boltzConstant / temperature * sqrt(boltzConstant * temperature / mass) * exp(-0.5) * ds;
	this->ds = (xMax - xMin) / (xAxis.getGlobalBounds().width - yAxis.getGlobalBounds().width);
	this->dy = (yMax - yMin) / (yAxis.getGlobalBounds().height - xAxis.getGlobalBounds().height);
	this->redrawBoltzPlot();
	this->updateAxes();
}

void Graph::initPlot() {
	for (int i = 0; i < xAxis.getGlobalBounds().width - yAxis.getGlobalBounds().width; i++) {
		boltzmannPlot.push_back(sf::RectangleShape());
		boltzmannPlot[i].setFillColor(sf::Color(255, 171, 0));
		boltzmannPlot[i].setSize(sf::Vector2f(1.f, 5.f));
		boltzmannPlot[i].setPosition(xAxis.getPosition().x + yAxis.getGlobalBounds().width, xAxis.getPosition().y - 0.5 * boltzmannPlot[0].getGlobalBounds().height);
		//Incorporate proportionality to the y axis to the code below
		boltzmannPlot[i].move(i, -mass * i * ds / (boltzConstant*temperature) * exp(-mass * pow(i * ds, 2) / (2 * boltzConstant * temperature)) * ds / dy);
	}
}

void Graph::initBars() {
	for (int i = 0; i < numberOfBars; i++) {
		bars.push_back(sf::RectangleShape());
		bars[i].setFillColor(sf::Color::Blue);
		bars[i].setPosition(xAxis.getPosition().x + yAxis.getGlobalBounds().width, xAxis.getPosition().y);
		bars[i].move(i * (xAxis.getGlobalBounds().width - yAxis.getGlobalBounds().width) / numberOfBars, 0);
		bars[i].setSize(sf::Vector2f((xAxis.getGlobalBounds().width - yAxis.getGlobalBounds().width) / numberOfBars, -10.f));
	}
	for (int i = 0; i < numberOfBars; i++) {
		currentDensities.push_back(0);
		averageDensities.push_back(0);
	}
}

void Graph::initNotches() {
	for (int i = 0; i < numberOfBars + 1; i++) {
		xNotches.push_back(sf::RectangleShape());
		xNotches[i].setFillColor(sf::Color::Black);
		xNotches[i].setSize(sf::Vector2f(10.f, 15.f));
		xNotches[i].setPosition(xAxis.getPosition().x, xAxis.getPosition().y + xAxis.getGlobalBounds().height);
		xNotches[i].move(i * (xAxis.getGlobalBounds().width-xNotches[0].getGlobalBounds().width)/numberOfBars, 0.f);
	}
	for (int i = 0; i < yNumberOfNumbers; i++) {
		yNotches.push_back(sf::RectangleShape());
		yNotches[i].setFillColor(sf::Color::Black);
		yNotches[i].setSize(sf::Vector2f(15.f, 10.f));
		yNotches[i].setPosition(yAxis.getPosition().x - yNotches[0].getGlobalBounds().width, yAxis.getPosition().y);
		yNotches[i].move(0.f, i * (yAxis.getGlobalBounds().height - yNotches[0].getGlobalBounds().height) / (yNumberOfNumbers-1));
	}
}

void Graph::initAxes() {
	this->xAxis.setFillColor(sf::Color::Black);
	this->xAxis.setPosition(sf::Vector2f(1100.f, 830.f));
	this->xAxis.setSize(sf::Vector2f(850.f, 10.f));

	this->yAxis.setFillColor(sf::Color::Black);
	this->yAxis.setPosition(sf::Vector2f(1100.f, 40.f));
	this->yAxis.setSize(sf::Vector2f(10.f, 800.f));
}

void Graph::initFonts() {
	this->font.loadFromFile("Fonts/design.graffiti.comicsansmsgras.ttf");
}

void Graph::initBalls() {
	for (int i = 0; i < numberOfBars; i++) {
		averageBalls.push_back(sf::CircleShape());
		averageBalls[i].setFillColor(sf::Color::Red);
		averageBalls[i].setRadius(5.f);
		averageBalls[i].setPosition(this->xAxis.getPosition().x + this->yAxis.getGlobalBounds().width, this->xAxis.getPosition().y - 0.5*this->averageBalls[0].getGlobalBounds().width);
		averageBalls[i].move((i+0.25)*(xAxis.getGlobalBounds().width-yAxis.getGlobalBounds().width) / (this->numberOfBars), 0);
	}
	this->yBaseline = this->xAxis.getPosition().y - 0.5 * this->averageBalls[0].getGlobalBounds().width;
}

void Graph::initText() {
	//Axes titles
	this->xAxisTitle.setFont(this->font);
	this->xAxisTitle.setCharacterSize(20.f);
	this->xAxisTitle.setFillColor(sf::Color::Black);
	this->xAxisTitle.setString("Velocity (m/s)");
	this->xAxisTitle.setPosition(sf::Vector2f(xAxis.getPosition().x + 0.5 * xAxis.getGlobalBounds().width - 0.5 * xAxisTitle.getGlobalBounds().width, xAxis.getPosition().y + 60.f));

	this->yAxisTitle.setFont(this->font);
	this->yAxisTitle.setCharacterSize(20.f);
	this->yAxisTitle.setFillColor(sf::Color::Black);
	this->yAxisTitle.setString("Frequency density");
	this->yAxisTitle.rotate(270);
	this->yAxisTitle.setPosition(sf::Vector2f(yAxis.getPosition().x - 70.f, yAxis.getPosition().y + 0.5 * yAxis.getGlobalBounds().height + 0.5* yAxisTitle.getGlobalBounds().height));

	//Axes values
	for (int i = 0; i < 8; i++) {
		this->xTextList.push_back(sf::Text());
		xTextList[i].setFont(this->font);
		xTextList[i].setFillColor(sf::Color::Black);
		xTextList[i].setCharacterSize(20.f);
		std::stringstream string;
		string << round(4000.f/7.f * float(i));
		xTextList[i].setString(string.str());
		xTextList[i].setPosition(this->xAxis.getPosition().x, this->xAxis.getPosition().y + xTextList[0].getGlobalBounds().height + 10.f);
	}
	for (int i = 0; i < 8; i++) {
		xTextList[i].move(i * (xAxis.getGlobalBounds().width) / 7 - 0.5*xTextList[i].getGlobalBounds().width, 0.f);
	}
	for (int i = 0; i < yNumberOfNumbers; i++) {
		this->yTextList.push_back(sf::Text());
		yTextList[i].setFont(this->font);
		yTextList[i].setString(std::to_string(yMin + i * (yMax - yMin) / (yNumberOfNumbers - 1)));
		yTextList[i].setFillColor(sf::Color::Black);
		yTextList[i].setCharacterSize(15.f);
		yTextList[i].rotate(270);
	}
	for (int i = 0; i < yNumberOfNumbers; i++) {
		yTextList[i].setPosition(this->yAxis.getPosition().x - 40, this->yAxis.getPosition().y + this->yAxis.getGlobalBounds().height - 0.5 * yTextList[0].getGlobalBounds().width);
		yTextList[i].move(0, -i * (yAxis.getGlobalBounds().height - yTextList[yNumberOfNumbers-1].getGlobalBounds().width) / (yNumberOfNumbers-1) + 0.5 * yTextList[i].getGlobalBounds().height);
	}
}

void Graph::render(sf::RenderTarget* target) {
	target->draw(this->xAxis);
	target->draw(this->yAxis);
	target->draw(this->xAxisTitle);
	target->draw(this->yAxisTitle);
	for (sf::Text i : xTextList) {
		target->draw(i);
	}
	for (sf::Text i : yTextList) {
		target->draw(i);
	}
	for (sf::RectangleShape i : xNotches) {
		target->draw(i);
	}
	for (sf::RectangleShape i : yNotches) {
		target->draw(i);
	}
	for (sf::RectangleShape i : bars) {
		target->draw(i);
	}
	for (sf::RectangleShape i : boltzmannPlot) {
		target->draw(i);
	}
	for (sf::CircleShape i : averageBalls) {
		target->draw(i);
	}
}

Graph::Graph() {
	this->initVariables();
	this->initNotches();
	this->initFonts();
	this->initText();
	this->initPlot();
	this->initBars();
	this->initBalls();
}

Graph::~Graph() {
}
