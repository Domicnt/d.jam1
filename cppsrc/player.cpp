#include "stdafx.h"

Player::Player(std::string id)
{
	x = 0;
	y = 0;
	Vx = 0;
	Vy = 0;

	r = 10;

	for (auto i = 0; i < 20; i++)
	{
		trailX[i] = 0;
		trailY[i] = 0;
	}

	score = 0;
	
	ID = id;

	std::string values = "6789abcdef";
	srand(time(nullptr));
	for(auto i = 0; i < 3; i++)
	{
		color += values.at(rand() % 10);
	}
}

Player::Player(double x1, double y1, double Vx1, double Vy1, std::string id)
{
	x = x1;
	y = y1;
	Vx = Vx1;
	Vy = Vy1;

	r = 10;

	for (auto i = 0; i < 20; i++)
	{
		trailX[i] = 0;
		trailY[i] = 0;
	}

	score = 0;

	ID = id;

	std::string values = "6789abcdef";
	srand(time(nullptr));
	for (auto i = 0; i < 3; i++)
	{
		color += values.at(rand() % 10);
	}
}