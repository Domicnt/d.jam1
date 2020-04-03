#include "stdafx.h"

Player::Player(std::string id)
{
	x = 0;
	y = 0;
	Vx = 0;
	Vy = 0;

	r = 10;

	ID = id;
}

Player::Player(double x1, double y1, double Vx1, double Vy1, std::string id)
{
	x = x1;
	y = y1;
	Vx = Vx1;
	Vy = Vy1;

	r = 10;

	ID = id;
}