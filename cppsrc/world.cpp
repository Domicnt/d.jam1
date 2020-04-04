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
		//drag
		i.Vx *= .995;
		i.Vy *= .995;

		//update trail positions
		i.trailX[0] = i.x;
		i.trailY[0] = i.y;
		for(auto j = std::size(i.trailX) - 1; j > 0; j--)
		{
			i.trailX[j] = i.trailX[j - 1];
			i.trailY[j] = i.trailY[j - 1];
		}

		//update score
		for(auto j : players)
		{
			if (i.ID == j.ID) continue;
			for(auto k = 0; k < std::size(j.trailX); k++)
			{
				if(sqrt(pow(i.x - j.trailX[k], 2) + pow(i.y - j.trailY[k], 2)) < i.r * 2)
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
		player.Set("color", Napi::String::New(env, players[i].color));
		player.Set("score", Napi::Number::New(env, players[i].score));

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

void World::playerCollision()
{
	for (auto i = 0; i < std::size(players); i++)
	{
		for(auto j = i; j < std::size(players); j++)
		{
			if(sqrt(pow(players[i].x - players[j].x, 2) + pow(players[i].y - players[j].y, 2)) < players[i].r + players[j].r)
			{
				//new velocities
				auto const newVelX1 = (players[i].Vx * (3.14 * pow(players[i].r, 2) - 3.14 * pow(players[j].r, 2)) + (2 * 3.14 * pow(players[j].r, 2) * players[j].Vx)) / (3.14 * pow(players[i].r, 2) + 3.14 * pow(players[j].r, 2));
				auto const newVelY1 = (players[i].Vy * (3.14 * pow(players[i].r, 2) - 3.14 * pow(players[j].r, 2)) + (2 * 3.14 * pow(players[j].r, 2) * players[j].Vy)) / (3.14 * pow(players[i].r, 2) + 3.14 * pow(players[j].r, 2));
				auto const newVelX2 = (players[j].Vx * (3.14 * pow(players[j].r, 2) - 3.14 * pow(players[i].r, 2)) + (2 * 3.14 * pow(players[i].r, 2) * players[i].Vx)) / (3.14 * pow(players[i].r, 2) + 3.14 * pow(players[j].r, 2));
				auto const newVelY2 = (players[j].Vy * (3.14 * pow(players[j].r, 2) - 3.14 * pow(players[i].r, 2)) + (2 * 3.14 * pow(players[i].r, 2) * players[i].Vy)) / (3.14 * pow(players[i].r, 2) + 3.14 * pow(players[j].r, 2));

				//update velocities
				players[i].Vx = newVelX1;
				players[i].Vy = newVelY1;
				players[j].Vx = newVelX2;
				players[j].Vy = newVelY2;

				//depth of collision
				auto const depth = (players[i].r + players[j].r) - sqrt(pow(players[i].x - players[j].x, 2) + pow(players[i].y - players[j].y, 2));

				//angle of collision
				auto const angle1 = atan2(players[i].y - players[j].y, players[i].x - players[j].x);

				//update positions
				players[i].x += cos(angle1) * depth / 2;
				players[i].y += sin(angle1) * depth / 2;
				players[j].x -= cos(angle1) * depth / 2;
				players[j].y -= sin(angle1) * depth / 2;
			}
		}
	}
}
