#pragma once
#include "stdafx.h"

class Player
{
public:
	//position and velocity
	double x, y, Vx, Vy;
	//radius
	double r;

	//identification string
	std::string ID;

	//constructors
	Player(std::string id);
	Player(double x1, double y1, double Vx1, double Vy1, std::string id);
};