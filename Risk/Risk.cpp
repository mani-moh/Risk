#include <Risk.h>
#include <cstdlib>
#include <algorithm>
#include <random>
void Player::addTroops(Game *risk) {
	troops += size(ownedTerritories) / 3;
	for (const auto& [continent,add] : risk->continentTroops) {
		bool contHeld = true;
		for (const auto& [tName, territory] : risk->territories) 
			if (territory.continent == continent && territory.owner->name != name)
				contHeld = false;
		if (contHeld)
			troops += add;
	}
}
void Player::removeTroops(int amount) {
	troops -= amount;
}
void Player::placeTroops(Territory& t, int amount) {
	t.addTroops(amount);
	this->removeTroops(amount);
}
void Player::moveTroops(Territory& t1, Territory& t2, int amount) {
	t1.removeTroops(amount);
	t2.addTroops(amount);
}
bool Player::hasPath(Territory* start, Territory* end, unordered_set<Territory*>& visited) {
	if (start == end) return true;
	visited.insert(start);

	// Explore neighbors
	for (Territory* neighbor : start->neighbors) {
		if (visited.count(neighbor) || neighbor->owner != this) continue;
		if (hasPath(neighbor, end, visited)) return true;
	}

	return false;
}



void Territory::addTroops(int amount) {
	troops += amount;
}
void Territory::removeTroops(int amount) {
	if (troops > amount)
		troops -= amount;
	else
		troops = 0;
}
void Territory::attack(Territory& t, int amount) {
	vector<int> att, def;
	int attackers = amount;
	int defenders = t.troops;
	int standoffs;
	if (defenders > 2) defenders = 2;
	if (attackers > 3) attackers = 3;
	for (int i = 0; i < attackers; i++)
		att.push_back(rand() % 6 + 1);
	for (int i = 0; i < defenders; i++)
		def.push_back(rand() % 6 + 1);
	sort(att.begin(), att.end());
	reverse(att.begin(), att.end());
	sort(def.begin(), def.end());
	reverse(def.begin(), def.end());
	if (attackers > defenders)
		standoffs = defenders;
	else
		standoffs = attackers;
	int attRoll = 0, defRoll = 0;
	for (int i = 0; i < standoffs; i++) {
		attRoll += att[i];
		defRoll += def[i];
	}
	if (attRoll > defRoll)
		t.removeTroops(attRoll - defRoll);
	else if (attRoll == defRoll)
		removeTroops(1);
	else
		if (troops - defRoll + attRoll > 0)
			removeTroops(defRoll - attRoll);
		else
			troops = 1;
	if (t.troops <= 0) {
		t.owner = owner;
		owner->ownedTerritories.push_back(&t);
		owner->moveTroops(*this, t, attackers);
	}
}
bool Territory::checkNeighbor(Territory& t) {
	for (auto territory : neighbors)
		if (territory->name == t.name)
			return true;
	return false;
}

void Game::nextPlayer() {
	phase.player = &players[(phase.player->num)%3 + 1];
}
void Game::init() {
	srand(time(0));
	InitWindow(1200, 900, "Risk");
	//continents----------------------------------------------------------------
	continentTroops = {
		{Africa,3},
		{Asia,7},
		{Europe,5},
		{NorthAmerica,5},
		{SouthAmerica,2},
		{Oceania,2}
	};
	
	//territories---------------------------------------------------------------
	territories = {
		{"Alaska", {"Alaska", {}, NorthAmerica, nullptr, 1}},
		{"Northwest Territory", {"Northwest Territory", {}, NorthAmerica, nullptr, 2}},
		{"Greenland", {"Greenland", {}, NorthAmerica, nullptr, 1}},
		{"Alberta", {"Alberta", {}, NorthAmerica, nullptr, 1}},
		{"Ontario", {"Ontario", {}, NorthAmerica, nullptr, 1}},
		{"Quebec", {"Quebec", {}, NorthAmerica, nullptr, 1}},
		{"Western United States", {"Western United States", {}, NorthAmerica, nullptr, 2}},
		{"Eastern United States", {"Eastern United States", {}, NorthAmerica, nullptr, 1}},
		{"Central America", {"Central America", {}, NorthAmerica, nullptr, 1}},
		{"Venezuela", {"Venezuela", {}, SouthAmerica, nullptr, 2}},
		{"Peru", {"Peru", {}, SouthAmerica, nullptr, 1}},
		{"Brazil", {"Brazil", {}, SouthAmerica, nullptr, 2}},
		{"Argentina", {"Argentina", {}, SouthAmerica, nullptr, 1}},
		{"Iceland", {"Iceland", {}, Europe, nullptr, 1}},
		{"Great Britain", {"Great Britain", {}, Europe, nullptr, 1}},
		{"Scandinavia", {"Scandinavia", {}, Europe, nullptr, 2}},
		{"Ukraine", {"Ukraine", {}, Europe, nullptr, 2}},
		{"Northern Europe", {"Northern Europe", {}, Europe, nullptr, 1}},
		{"Western Europe", {"Western Europe", {}, Europe, nullptr, 1}},
		{"Southern Europe", {"Southern Europe", {}, Europe, nullptr, 1}},
		{"North Africa", {"North Africa", {}, Africa, nullptr, 1}},
		{"Egypt", {"Egypt", {}, Africa, nullptr, 1}},
		{"East Africa", {"East Africa", {}, Africa, nullptr, 2}},
		{"Congo", {"Congo", {}, Africa, nullptr, 1}},
		{"South Africa", {"South Africa", {}, Africa, nullptr, 2}},
		{"Madagascar", {"Madagascar", {}, Africa, nullptr, 1}},
		{"Ural", {"Ural", {}, Asia, nullptr, 1}},
		{"Siberia", {"Siberia", {}, Asia, nullptr, 1}},
		{"Yakutsk", {"Yakutsk", {}, Asia, nullptr, 1}},
		{"Irkutsk", {"Irkutsk", {}, Asia, nullptr, 1}},
		{"Kamchatka", {"Kamchatka", {}, Asia, nullptr, 1}},
		{"Japan", {"Japan", {}, Asia, nullptr, 1}},
		{"Siam", {"Siam", {}, Asia, nullptr, 2}},
		{"China", {"China", {}, Asia, nullptr, 1}},
		{"Mongolia", {"Mongolia", {}, Asia, nullptr, 2}},
		{"Afghanistan", {"Afghanistan", {}, Asia, nullptr, 1}},
		{"India", {"India", {}, Asia, nullptr, 1}},
		{"Middle East", {"Middle East", {}, Asia, nullptr, 2}},
		{"Indonesia", {"Indonesia", {}, Oceania, nullptr, 1}},
		{"New Guinea", {"New Guinea", {}, Oceania, nullptr, 1}},
		{"Eastern Australia", {"Eastern Australia", {}, Oceania, nullptr, 1}},
		{"Western Australia", {"Western Australia", {}, Oceania, nullptr, 2}}
		};

	//territoryrects------------------------------------------------------------
	territoryRects = {
		// North America
		{"Alaska", {40, 40, 40, 40}},
		{"Northwest Territory", {80, 40, 120, 40}},
		{"Greenland", {240, 0, 80, 80}},
		{"Alberta", {40, 80, 80, 40}},
		{"Ontario", {120, 80, 80, 80}},
		{"Quebec", {200, 120, 80, 40}},
		{"Western United States", {40, 120, 80, 80}},
		{"Eastern United States", {120, 160, 160, 60}},
		{"Central America", {80, 200, 40, 80}},

		// South America
		{"Venezuela", {80, 280, 120, 40}},
		{"Peru", {80, 320, 40, 120}},
		{"Brazil", {120, 320, 120, 120}},
		{"Argentina", {80, 440, 80, 120}},

		// Europe
		{"Iceland", {400, 40, 40, 40}},
		{"Great Britain", {400, 120, 40, 80}},
		{"Scandinavia", {480, 40, 80, 80}},
		{"Ukraine", {560, 40, 120, 180}},
		{"Northern Europe", {480, 160, 80, 60}},
		{"Western Europe", {400, 240, 40, 80}},
		{"Southern Europe", {440, 220, 160, 80}},

		// Africa
		{"North Africa", {400, 320, 80, 80}},
		{"Egypt", {480, 320, 80, 40}},
		{"East Africa", {480, 360, 80, 80}},
		{"Congo", {440, 400, 40, 40}},
		{"South Africa", {440, 440, 80, 80}},
		{"Madagascar", {600, 440, 40, 80}},

		// Asia
		{"Ural", {680, 40, 80, 140}},
		{"Siberia", {760, 40, 40, 120}},
		{"Yakutsk", {800, 40, 80, 40}},
		{"Irkutsk", {800, 80, 80, 40}},
		{"Kamchatka", {880, 40, 40, 80}},
		{"Japan", {960, 80, 40, 120}},
		{"Siam", {760, 320, 120, 40}},
		{"China", {760, 160, 120, 160}},
		{"Mongolia", {800, 120, 120, 40}},
		{"Afghanistan", {680, 180, 80, 60}},
		{"India", {680, 240, 80, 120}},
		{"Middle East", {600, 220, 80, 140}},

		// Oceania
		{"Indonesia", {840, 400, 80, 80}},
		{"New Guinea", {960, 400, 40, 40}},
		{"Eastern Australia", {960, 520, 80, 80}},
		{"Western Australia", {880, 520, 80, 80}}
	};

	//connections---------------------------------------------------------------
	{
		territories["Alaska"].neighbors.push_back(&territories["Northwest Territory"]);
		territories["Alaska"].neighbors.push_back(&territories["Alberta"]);
		territories["Alaska"].neighbors.push_back(&territories["Kamchatka"]);

		territories["Greenland"].neighbors.push_back(&territories["Quebec"]);
		territories["Greenland"].neighbors.push_back(&territories["Northwest Territory"]);
		territories["Greenland"].neighbors.push_back(&territories["Ontario"]);
		territories["Greenland"].neighbors.push_back(&territories["Iceland"]);

		territories["Ontario"].neighbors.push_back(&territories["Greenland"]);
		territories["Ontario"].neighbors.push_back(&territories["Quebec"]);
		territories["Ontario"].neighbors.push_back(&territories["Alberta"]);
		territories["Ontario"].neighbors.push_back(&territories["Western United States"]);
		territories["Ontario"].neighbors.push_back(&territories["Northwest Territory"]);
		territories["Ontario"].neighbors.push_back(&territories["Eastern United States"]);

		territories["Quebec"].neighbors.push_back(&territories["Ontario"]);
		territories["Quebec"].neighbors.push_back(&territories["Greenland"]);
		territories["Quebec"].neighbors.push_back(&territories["Eastern United States"]);

		territories["Northwest Territory"].neighbors.push_back(&territories["Greenland"]);
		territories["Northwest Territory"].neighbors.push_back(&territories["Ontario"]);
		territories["Northwest Territory"].neighbors.push_back(&territories["Alaska"]);
		territories["Northwest Territory"].neighbors.push_back(&territories["Alberta"]);

		territories["Alberta"].neighbors.push_back(&territories["Northwest Territory"]);
		territories["Alberta"].neighbors.push_back(&territories["Ontario"]);
		territories["Alberta"].neighbors.push_back(&territories["Western United States"]);
		territories["Alberta"].neighbors.push_back(&territories["Alaska"]);

		territories["Western United States"].neighbors.push_back(&territories["Alberta"]);
		territories["Western United States"].neighbors.push_back(&territories["Central America"]);
		territories["Western United States"].neighbors.push_back(&territories["Ontario"]);
		territories["Western United States"].neighbors.push_back(&territories["Eastern United States"]);

		territories["Eastern United States"].neighbors.push_back(&territories["Ontario"]);
		territories["Eastern United States"].neighbors.push_back(&territories["Western United States"]);
		territories["Eastern United States"].neighbors.push_back(&territories["Central America"]);
		territories["Eastern United States"].neighbors.push_back(&territories["Quebec"]);

		territories["Central America"].neighbors.push_back(&territories["Western United States"]);
		territories["Central America"].neighbors.push_back(&territories["Eastern United States"]);
		territories["Central America"].neighbors.push_back(&territories["Venezuela"]);

		territories["Venezuela"].neighbors.push_back(&territories["Central America"]);
		territories["Venezuela"].neighbors.push_back(&territories["Peru"]);
		territories["Venezuela"].neighbors.push_back(&territories["Brazil"]);

		territories["Peru"].neighbors.push_back(&territories["Venezuela"]);
		territories["Peru"].neighbors.push_back(&territories["Brazil"]);
		territories["Peru"].neighbors.push_back(&territories["Argentina"]);

		territories["Argentina"].neighbors.push_back(&territories["Peru"]);
		territories["Argentina"].neighbors.push_back(&territories["Brazil"]);

		territories["Brazil"].neighbors.push_back(&territories["Peru"]);
		territories["Brazil"].neighbors.push_back(&territories["Argentina"]);
		territories["Brazil"].neighbors.push_back(&territories["Venezuela"]);
		territories["Brazil"].neighbors.push_back(&territories["North Africa"]);

		territories["North Africa"].neighbors.push_back(&territories["Brazil"]);
		territories["North Africa"].neighbors.push_back(&territories["Egypt"]);
		territories["North Africa"].neighbors.push_back(&territories["East Africa"]);
		territories["North Africa"].neighbors.push_back(&territories["Congo"]);
		territories["North Africa"].neighbors.push_back(&territories["Western Europe"]);

		territories["Egypt"].neighbors.push_back(&territories["North Africa"]);
		territories["Egypt"].neighbors.push_back(&territories["East Africa"]);
		territories["Egypt"].neighbors.push_back(&territories["Southern Europe"]);
		territories["Egypt"].neighbors.push_back(&territories["Middle East"]);

		territories["East Africa"].neighbors.push_back(&territories["North Africa"]);
		territories["East Africa"].neighbors.push_back(&territories["Egypt"]);
		territories["East Africa"].neighbors.push_back(&territories["Congo"]);
		territories["East Africa"].neighbors.push_back(&territories["South Africa"]);
		territories["East Africa"].neighbors.push_back(&territories["Madagascar"]);
		territories["East Africa"].neighbors.push_back(&territories["Middle East"]);

		territories["South Africa"].neighbors.push_back(&territories["East Africa"]);
		territories["South Africa"].neighbors.push_back(&territories["Madagascar"]);
		territories["South Africa"].neighbors.push_back(&territories["Congo"]);

		territories["Madagascar"].neighbors.push_back(&territories["East Africa"]);
		territories["Madagascar"].neighbors.push_back(&territories["South Africa"]);

		territories["Congo"].neighbors.push_back(&territories["East Africa"]);
		territories["Congo"].neighbors.push_back(&territories["South Africa"]);
		territories["Congo"].neighbors.push_back(&territories["North Africa"]);

		territories["Middle East"].neighbors.push_back(&territories["Egypt"]);
		territories["Middle East"].neighbors.push_back(&territories["East Africa"]);
		territories["Middle East"].neighbors.push_back(&territories["India"]);
		territories["Middle East"].neighbors.push_back(&territories["Southern Europe"]);
		territories["Middle East"].neighbors.push_back(&territories["Ukraine"]);
		territories["Middle East"].neighbors.push_back(&territories["Afghanistan"]);

		territories["Afghanistan"].neighbors.push_back(&territories["Middle East"]);
		territories["Afghanistan"].neighbors.push_back(&territories["Ukraine"]);
		territories["Afghanistan"].neighbors.push_back(&territories["Ural"]);
		territories["Afghanistan"].neighbors.push_back(&territories["India"]);
		territories["Afghanistan"].neighbors.push_back(&territories["China"]);

		territories["India"].neighbors.push_back(&territories["Middle East"]);
		territories["India"].neighbors.push_back(&territories["Afghanistan"]);
		territories["India"].neighbors.push_back(&territories["Siam"]);
		territories["India"].neighbors.push_back(&territories["China"]);

		territories["China"].neighbors.push_back(&territories["Afghanistan"]);
		territories["China"].neighbors.push_back(&territories["India"]);
		territories["China"].neighbors.push_back(&territories["Siam"]);
		territories["China"].neighbors.push_back(&territories["Ural"]);
		territories["China"].neighbors.push_back(&territories["Siberia"]);
		territories["China"].neighbors.push_back(&territories["Mongolia"]);

		territories["Siberia"].neighbors.push_back(&territories["China"]);
		territories["Siberia"].neighbors.push_back(&territories["Mongolia"]);
		territories["Siberia"].neighbors.push_back(&territories["Ural"]);
		territories["Siberia"].neighbors.push_back(&territories["Irkutsk"]);
		territories["Siberia"].neighbors.push_back(&territories["Yakutsk"]);

		territories["Ural"].neighbors.push_back(&territories["Siberia"]);
		territories["Ural"].neighbors.push_back(&territories["China"]);
		territories["Ural"].neighbors.push_back(&territories["Afghanistan"]);
		territories["Ural"].neighbors.push_back(&territories["Ukraine"]);

		territories["Siam"].neighbors.push_back(&territories["China"]);
		territories["Siam"].neighbors.push_back(&territories["India"]);
		territories["Siam"].neighbors.push_back(&territories["Indonesia"]);

		territories["Yakutsk"].neighbors.push_back(&territories["Irkutsk"]);
		territories["Yakutsk"].neighbors.push_back(&territories["Siberia"]);
		territories["Yakutsk"].neighbors.push_back(&territories["Kamchatka"]);

		territories["Irkutsk"].neighbors.push_back(&territories["Yakutsk"]);
		territories["Irkutsk"].neighbors.push_back(&territories["Siberia"]);
		territories["Irkutsk"].neighbors.push_back(&territories["Kamchatka"]);
		territories["Irkutsk"].neighbors.push_back(&territories["Mongolia"]);

		territories["Mongolia"].neighbors.push_back(&territories["China"]);
		territories["Mongolia"].neighbors.push_back(&territories["Irkutsk"]);
		territories["Mongolia"].neighbors.push_back(&territories["Kamchatka"]);
		territories["Mongolia"].neighbors.push_back(&territories["Japan"]);
		territories["Mongolia"].neighbors.push_back(&territories["Siberia"]);

		territories["Kamchatka"].neighbors.push_back(&territories["Yakutsk"]);
		territories["Kamchatka"].neighbors.push_back(&territories["Irkutsk"]);
		territories["Kamchatka"].neighbors.push_back(&territories["Mongolia"]);
		territories["Kamchatka"].neighbors.push_back(&territories["Alaska"]);
		territories["Kamchatka"].neighbors.push_back(&territories["Japan"]);

		territories["Japan"].neighbors.push_back(&territories["Kamchatka"]);
		territories["Japan"].neighbors.push_back(&territories["Mongolia"]);

		territories["Indonesia"].neighbors.push_back(&territories["Siam"]);
		territories["Indonesia"].neighbors.push_back(&territories["New Guinea"]);
		territories["Indonesia"].neighbors.push_back(&territories["Western Australia"]);

		territories["New Guinea"].neighbors.push_back(&territories["Indonesia"]);
		territories["New Guinea"].neighbors.push_back(&territories["Western Australia"]);
		territories["New Guinea"].neighbors.push_back(&territories["Eastern Australia"]);

		territories["Western Australia"].neighbors.push_back(&territories["Indonesia"]);
		territories["Western Australia"].neighbors.push_back(&territories["New Guinea"]);
		territories["Western Australia"].neighbors.push_back(&territories["Eastern Australia"]);

		territories["Eastern Australia"].neighbors.push_back(&territories["Western Australia"]);
		territories["Eastern Australia"].neighbors.push_back(&territories["New Guinea"]);

		territories["Ukraine"].neighbors.push_back(&territories["Ural"]);
		territories["Ukraine"].neighbors.push_back(&territories["Afghanistan"]);
		territories["Ukraine"].neighbors.push_back(&territories["Southern Europe"]);
		territories["Ukraine"].neighbors.push_back(&territories["Northern Europe"]);
		territories["Ukraine"].neighbors.push_back(&territories["Scandinavia"]);
		territories["Ukraine"].neighbors.push_back(&territories["Middle East"]);

		territories["Northern Europe"].neighbors.push_back(&territories["Ukraine"]);
		territories["Northern Europe"].neighbors.push_back(&territories["Southern Europe"]);
		territories["Northern Europe"].neighbors.push_back(&territories["Western Europe"]);
		territories["Northern Europe"].neighbors.push_back(&territories["Scandinavia"]);
		territories["Northern Europe"].neighbors.push_back(&territories["Great Britain"]);

		territories["Scandinavia"].neighbors.push_back(&territories["Ukraine"]);
		territories["Scandinavia"].neighbors.push_back(&territories["Northern Europe"]);
		territories["Scandinavia"].neighbors.push_back(&territories["Iceland"]);
		territories["Scandinavia"].neighbors.push_back(&territories["Great Britain"]);

		territories["Great Britain"].neighbors.push_back(&territories["Northern Europe"]);
		territories["Great Britain"].neighbors.push_back(&territories["Scandinavia"]);
		territories["Great Britain"].neighbors.push_back(&territories["Western Europe"]);
		territories["Great Britain"].neighbors.push_back(&territories["Iceland"]);

		territories["Iceland"].neighbors.push_back(&territories["Scandinavia"]);
		territories["Iceland"].neighbors.push_back(&territories["Greenland"]);
		territories["Iceland"].neighbors.push_back(&territories["Great Britain"]);

		territories["Southern Europe"].neighbors.push_back(&territories["Northern Europe"]);
		territories["Southern Europe"].neighbors.push_back(&territories["Western Europe"]);
		territories["Southern Europe"].neighbors.push_back(&territories["Egypt"]);
		territories["Southern Europe"].neighbors.push_back(&territories["Middle East"]);
		territories["Southern Europe"].neighbors.push_back(&territories["Ukraine"]);

		territories["Western Europe"].neighbors.push_back(&territories["Northern Europe"]);
		territories["Western Europe"].neighbors.push_back(&territories["Southern Europe"]);
		territories["Western Europe"].neighbors.push_back(&territories["North Africa"]);
		territories["Western Europe"].neighbors.push_back(&territories["Great Britain"]);
	}

	// Create a card for each territory with stars equal to starting troops
	for (auto& [name, territory] : territories) {
		
		Card card;
		card.territory = &territory;
		card.stars = territory.troops;
		deck.push_back(card);
	}

	//shuffle cards
	vector<string> names;
	for (const auto& [name, territory] : territories) {
		names.push_back(name);
	}
	random_device rd;
	mt19937 generator(rd());
	shuffle(names.begin(), names.end(), generator);
	
	//give starting troops
	for (int i = 0; i < 42; i++) {
		players[(i % 3) + 1].ownedTerritories.push_back(&territories[names[i]]);
		territories[names[i]].owner = &players[(i % 3) + 1];
	}
	players[1].addTroops(this);
}

