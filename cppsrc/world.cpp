#include "stdafx.h"

World::World(int const& width1, int const& height1)
{
	width = width1;
	height = height1;
}


Napi::Object World::step(Napi::Env env)
{
	for (auto& i : players)
	{
		//gravity
		i.Vy += .1;

		//drag
		i.Vx *= .995;
		i.Vy *= .995;
		
		//velocities
		i.x += i.Vx;
		i.y += i.Vy;
	}
	edgeCollision();
	Napi::Object world = Napi::Object::New(env);
	world.Set("width", Napi::Number::New( env, width));
	world.Set("height", Napi::Number::New(env, height));
	Napi::Array jsplayers = Napi::Array::New(env);
	for (auto i  = 0; i < std::size(players); i++)
	{
		Napi::Object player = Napi::Object::New(env);;
		player.Set("x", Napi::Number::New(env, players[i].x));
		player.Set("y", Napi::Number::New(env, players[i].y));
		player.Set("Vx", Napi::Number::New(env, players[i].Vx));
		player.Set("Vy", Napi::Number::New(env, players[i].Vy));
		player.Set("r", Napi::Number::New(env, players[i].r));

		jsplayers.Set(i, player);
	}
	world.Set("players", jsplayers);
	return world;
}

void World::addPlayer(std::string id)
{
	players.emplace_back(id);
}

void World::accelPlayer(double x, double y, std::string id)
{
	for (auto i = 0; i < std::size(players); i++)
	{
		if (players[i].ID == id)
		{
			players[i].Vx += x;
			players[i].Vy += y;
		}
	}
}

void World::removePlayer(std::string id)
{
	for(auto i = 0; i < std::size(players); i++)
	{
		if(players[i].ID == id)
		{
			players.erase(players.begin() + i);
		}
	}
}

void World::edgeCollision()
{
	for (auto& i : players)
	{
		if (i.x - i.r < 0)
		{
			i.x = i.r;
			i.Vx = abs(i.Vx);
		}
		else if (i.x + i.r > width)
		{
			i.x = width - i.r;
			i.Vx = -abs(i.Vx);

		}
		if (i.y - i.r < 0)
		{
			i.y = i.r;
			i.Vy = abs(i.Vy);
		}
		else if (i.y + i.r > height)
		{
			i.y = height - i.r;
			i.Vy = -abs(i.Vy);
		}
		
	}
}
