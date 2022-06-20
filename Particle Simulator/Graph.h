#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <iostream>
#include <sstream>
#include <math.h> 

class Graph {
private:
	sf::Font font;
	sf::Text xAxisTitle, yAxisTitle;
	std::vector<sf::Text> xTextList;
	std::vector<sf::Text> yTextList;
	std::vector<sf::RectangleShape> xNotches, yNotches;
	std::vector<sf::RectangleShape> boltzmannPlot;
	void initPlot();
	void initBars();
	void initNotches();
	void initAxes();
	void initBalls();
	void initFonts();
	void initText();
	void initVariables();
public:
	std::vector<float> currentDensities;
	std::vector<float> averageDensities;
	std::vector<sf::RectangleShape> bars;
	std::vector<sf::CircleShape> averageBalls;
	void updateGraphParams(); //Is called when the user changes temperature or mass
	void redrawBoltzPlot(); //Redraws the orange line showing the Maxwell-Boltzmann distribution when the user changes parameters
	void updateAxes(); //Changes the numbers on the axes
	int resetStep = 0;
	float yBaseline;
	float mass;
	float temperature;
	float boltzConstant;
	float ds, dy;
	int xMin, xMax;
	float yMin, yMax;
	int numberOfBars, xNumberOfNumbers, yNumberOfNumbers;
	sf::RectangleShape xAxis, yAxis;
	void render(sf::RenderTarget* target);
	Graph();
	~Graph();
};

