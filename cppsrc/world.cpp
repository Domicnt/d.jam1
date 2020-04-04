#include "stdafx.h"
#include "cstdlib"

World::World(int const& width1, int const& height1)
{
	width = width1;
	height = height1;
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
				if ((i.x - j.trailX[k]) * (i.x - j.trailX[k]) + (i.y - j.trailY[k]) * (i.y - j.trailY[k]) < (i.r * 2) * (i.r * 2))
				{
					i.score++;
				}
			}
		}

		//velocities
		i.x += i.Vx;
		i.y += i.Vy;
	}
	playerCollision();
	edgeCollision();
	Napi::Object world = Napi::Object::New(env);
	world.Set("width", Napi::Number::New(env, width));
	world.Set("height", Napi::Number::New(env, height));
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

		jsplayers.Set(increment, player);
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
	for (auto &i : players)
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

void World::playerCollision()
{
	for (auto& i : players)
	{
		for (auto& j : players)
		{
			if (i.ID == j.ID) continue;
			if ((i.x - j.x) * (i.x - j.x) + (i.y - j.y) * (i.y - j.y) < (i.r + j.r) * (i.r + j.r))
			{
				//new velocities
				auto const newVelX1 = (i.Vx * (3.14 * (i.r * i.r) - 3.14 * (j.r * j.r)) + (2 * 3.14 * (j.r * j.r) * j.Vx)) / (3.14 * (i.r * i.r) + 3.14 * (j.r * j.r));
				auto const newVelY1 = (i.Vy * (3.14 * (i.r * i.r) - 3.14 * (j.r * j.r)) + (2 * 3.14 * (j.r * j.r) * j.Vy)) / (3.14 * (i.r * i.r) + 3.14 * (j.r * j.r));
				auto const newVelX2 = (j.Vx * (3.14 * (j.r * j.r) - 3.14 * (i.r * i.r)) + (2 * 3.14 * (j.r * j.r) * i.Vx)) / (3.14 * (i.r * i.r) + 3.14 * (j.r * j.r));
				auto const newVelY2 = (j.Vy * (3.14 * (j.r * j.r) - 3.14 * (i.r * i.r)) + (2 * 3.14 * (j.r * j.r) * i.Vy)) / (3.14 * (i.r * i.r) + 3.14 * (j.r * j.r));

				//update velocities
				i.Vx = newVelX1;
				i.Vy = newVelY1;
				j.Vx = newVelX2;
				j.Vy = newVelY2;

				//update positions
				i.x += (i.x - j.x) / 2;
				i.y += (i.y - j.y) / 2;
				j.x += (j.x - i.x) / 2;
				j.y += (j.y - i.y) / 2;
			}
		}
	}
}
