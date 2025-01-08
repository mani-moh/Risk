#pragma once
#include <raylib.h>
#include <string>
#include <vector>
#include <ctime>
#include <unordered_map>
#include <unordered_set>
using namespace std;



enum Continent {
	Africa, Asia, Europe, NorthAmerica, SouthAmerica, Oceania
};


struct Territory;
struct Player;
struct Game;

enum GamePhase {
	placement, attack, move
};

struct Phase {
	Player* player;
	string gamePhase;
};

enum ButtonState {
	off, on, hover, pressed
};

struct Card {
	Territory* territory;
	int stars;
};

struct Player {
	string name;
	vector<Territory*> ownedTerritories;
	vector<Card*> cards;
	Color color;
	int troops;
	int num;
	void addTroops(Game *risk);
	void removeTroops(int amount);
	void placeTroops(Territory& t, int amount);
	void moveTroops(Territory& t1, Territory& t2, int amount);
	bool hasPath(Territory* start, Territory* end, unordered_set<Territory*>& visited);
};

struct Territory {
	string name;
	vector<Territory*> neighbors;
	Continent continent;
	Player* owner;
	int troops;
	void addTroops(int amount);
	void removeTroops(int amount);
	void attack(Territory& t, int amount);
	bool checkNeighbor(Territory& t);
};

struct Line {
	Vector2 start;
	Vector2 end;
};

struct Button {
	string text;
	float x;
	float y;
	float width;
	float height;
	ButtonState state ;
};

struct Game {
	void init();
	void nextPlayer();
	Color backgroundColor = { 107, 95, 51, 255 };
	Color lineColor = { 0,0,0,255 };
	Color p1color = { 255, 0, 0, 255 };
	Color p2color = { 0, 0, 255, 255 };
	Color p3color = { 0, 255, 0, 255 };


	unordered_map<int, Player> players = {
		{1,{"player1", {}, {}, p1color, 0,1}},
		{2,{"player2", {}, {}, p2color, 0,2}},
		{3,{"player3", {}, {}, p3color, 0,3}},
	};
	unordered_map<string, Territory> territories;
	vector<Card> deck;
	unordered_map<string, Rectangle> territoryRects;
	unordered_map<Continent, int> continentTroops;
	
	Phase phase = {&players[1], "placement"};
};
