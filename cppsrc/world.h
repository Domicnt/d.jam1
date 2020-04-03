#pragma once

#include "stdafx.h"

class World
{
	std::vector<Player> players;
	int width, height;
public:
	//constructor
	World() = default;
	World(int const& width, int const& height);
	
	//one game step, returns everything for js
	Napi::Object step(Napi::Env env);
	//add a player
	void addPlayer(std::string id);
	//accellerate a player
	void accelPlayer(double x, double y, std::string id);
	//remove a player
	void removePlayer(std::string id);
	//collision with the edges
	void edgeCollision();
};