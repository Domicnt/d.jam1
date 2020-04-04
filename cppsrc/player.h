#pragma once
#include "stdafx.h"

class Player
{
public:
	//position and velocity
	double x, y, Vx, Vy;
	//radius
	double r;

	//trail point positions
	double trailX[20];
	double trailY[20];

	//score
	int score;
	
	//identification string
	std::string ID;

	//color it's drawn as
	std::string color;

	//constructors
	Player(std::string id);
	Player(double x1, double y1, double Vx1, double Vy1, std::string id);
};