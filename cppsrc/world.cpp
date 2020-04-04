#include "stdafx.h"
#include "cstdlib"

World::World(int const& width1, int const& height1)
{
	width = width1;
	height = height1;
	goalHeight = height / 2;
	srand(time(nullptr));
}


Napi::Object World::step(Napi::Env env)
{
	for (auto& i : players)
	{
		//drag
		i.Vx *= .995;
		i.Vy *= .995;

		//update trail positions
		i.trailX[0] = i.x;
		i.trailY[0] = i.y;
		for (auto j = 19; j > 0; j--)
		{
			i.trailX[j] = i.trailX[j - 1];
			i.trailY[j] = i.trailY[j - 1];
		}

		//update score
		for (auto j : players)
		{
			if (i.ID == j.ID) continue;
			for (auto k = 0; k < 20; k++)
			{
				if ((i.x - j.trailX[k]) * (i.x - j.trailX[k]) + (i.y - j.trailY[k]) * (i.y - j.trailY[k]) < (i.r  + j.r) * (i.r + j.r))
				{
					i.score++;
				}
			}
		}

		//update size
		i.r = 10 + i.score / 2500;

		//velocities
		i.x += i.Vx;
		i.y += i.Vy;
	}
	playerCollision();
	edgeCollision();
	Napi::Object world = Napi::Object::New(env);
	world.Set("width", Napi::Number::New(env, width));
	world.Set("height", Napi::Number::New(env, height));
	world.Set("goalHeight", Napi::Number::New(env, goalHeight));
	Napi::Array jsplayers = Napi::Array::New(env);
	int increment = -1;
	for (auto i : players)
	{
		increment++;
		Napi::Object player = Napi::Object::New(env);;
		player.Set("x", Napi::Number::New(env, i.x));
		player.Set("y", Napi::Number::New(env, i.y));
		player.Set("Vx", Napi::Number::New(env, i.Vx));
		player.Set("Vy", Napi::Number::New(env, i.Vy));
		player.Set("r", Napi::Number::New(env, i.r));
		player.Set("color", Napi::String::New(env, i.color));
		player.Set("score", Napi::Number::New(env, i.score));
		Napi::Array trailX = Napi::Array::New(env);
		Napi::Array trailY = Napi::Array::New(env);
		for(auto j = 0; j < 20; j++)
		{
			trailX.Set(j, i.trailX[j]);
			trailY.Set(j, i.trailY[j]);
		}
		player.Set("trailX", trailX);
		player.Set("trailY", trailY);

		jsplayers.Set(increment, player);
	}
	world.Set("players", jsplayers);
	return world;
}

void World::addPlayer(std::string id)
{
	players.emplace_back(id, width, height);
}

void World::accelPlayer(double x, double y, std::string id)
{
	for (auto& i : players)
	{
		if (i.ID == id)
		{
			i.Vx += x;
			i.Vy += y;
		}
	}
}

void World::removePlayer(std::string id)
{
	auto increment = -1;
	for (auto i : players)
	{
		increment++;
		if (i.ID == id)
		{
			players.erase(players.begin() + increment);
			break;
		}
	}
}

void World::edgeCollision()
{
	for (auto& i : players)
	{
		if (i.x - i.r < 0)
		{
			if(i.y > (height - goalHeight) / 2 && i.y < height - (height - goalHeight) / 2)
			{
				i = { i.ID, width, height };
				continue;
			}
			i.x = i.r;
			i.Vx = abs(i.Vx);
		}
		else if (i.x + i.r > width)
		{
			if (i.y > (height - goalHeight) / 2 && i.y < height - (height - goalHeight) / 2)
			{
				i = { i.ID, width, height };
				continue;
			}
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

void World::playerCollision()
{
	for (auto& i : players)
	{
		for (auto& j : players)
		{
			if (i.ID == j.ID) continue;
			if ((i.x - j.x) * (i.x - j.x) + (i.y - j.y) * (i.y - j.y) < (i.r + j.r) * (i.r + j.r))
			{
				i.Vx += (3.14 * (j.r * j.r) / (i.x - j.x)) / 1000;
				i.Vy += (3.14 * (j.r * j.r) / (i.y - j.y)) / 1000;
			}
		}
	}
}
