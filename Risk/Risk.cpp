#include "Risk.h"
#include <algorithm>
#include <random>




Territory::Territory(string n, vector<string> nb, float xf, float yf, float wf, float hf) {
	name = n;
	neighbors = nb;
	x = xf;
	y = yf;
	w = wf;
	h = hf;
	owner = 1;
	troops = 1;
}
Button::Button(string n, Rectangle r,int s, Color bg, Color t) {
	buttonText = n;
	buttonRec = r;
	textPos = {r.x + r.width/2, r.y + r.height/2};
	fontSize = s;
	color = bg;
	textColor = t;
}

//-------------------------------------------------------------------------------
Game::Game() {
	running = true;
	waited3s = false;
	conquered1 = false;
	cardsDrawn = false;
	usedCardsStars = 0;
	currentPlayerTroops = 0;
	bgColor = { 100, 149, 237 , 255 };
	pColor[1] = { 255,0,0,255 };
	pColor[2] = { 0,255,0,255 };
	pColor[3] = YELLOW;
	playerCount = 3;
	territoryCount = 42;
	currentPlayer = 1;
	isTerClicked = false;
	terClicked = "";
	attTer = "";
	attTar = "";
	blitz = false;
	forCount = 1;
	isAlive[1] = true;
	isAlive[2] = true;
	isAlive[3] = true;
	gamePhase = Menu;
	playerTerritoryCount = 0;
	territory = {
		{"Alaska", Territory("Alaska", {"Northwest Territory","Alberta","Kamchatka"}, 40,40,40,40) },
		{"Northwest Territory", Territory("Northwest Territory", {"Greenland","Ontario","Alaska","Alberta"},80, 40, 120, 40) },
		{"Greenland", Territory("Greenland", {"Quebec","Northwest Territory","Ontario","Iceland"},240, 5, 80, 75) },
		{"Alberta", Territory("Alberta", {"Northwest Territory", "Ontario", "Western United States", "Alaska"},40, 80, 80, 40) },
		{"Ontario", Territory("Ontario", {"Greenland", "Quebec", "Alberta", "Western United States", "Northwest Territory", "Eastern United States"},120, 80, 80, 80)},
		{"Quebec", Territory("Quebec", {"Ontario", "Greenland", "Eastern United States"},200, 120, 80, 40) },
		{"Western United States", Territory("Western United States", {"Alberta", "Central America", "Ontario", "Eastern United States"},40, 120, 80, 80) },
		{"Eastern United States", Territory("Eastern United States", {"Ontario", "Western United States", "Central America", "Quebec"},120, 160, 160, 60) },
		{"Central America", Territory("Central America", {"Western United States", "Eastern United States", "Venezuela"},80, 200, 40, 80) },

		{"Venezuela", Territory("Venezuela", {"Central America", "Peru", "Brazil"},80, 280, 120, 40) },
		{"Peru", Territory("Peru", {"Venezuela", "Brazil", "Argentina"},80, 320, 40, 120) },
		{"Brazil", Territory("Brazil", {"Peru", "Argentina", "Venezuela", "North Africa"},120, 320, 120, 120) },
		{"Argentina", Territory("Argentina", {"Peru", "Brazil"}, 80, 440, 80, 120) },

		{"Iceland", Territory("Iceland", {"Scandinavia", "Greenland", "Great Britain"}, 400, 40, 40, 40) },
		{"Great Britain", Territory("Great Britain", {"Northern Europe", "Scandinavia", "Western Europe", "Iceland"}, 400, 120, 40, 80) },
		{"Scandinavia", Territory("Scandinavia", {"Ukraine", "Northern Europe", "Iceland", "Great Britain"}, 480, 40, 80, 80) },
		{"Ukraine", Territory("Ukraine", {"Ural", "Afghanistan", "Southern Europe", "Northern Europe", "Scandinavia", "Middle East"}, 560, 40, 120, 180) },
		{"Northern Europe", Territory("Northern Europe", {"Ukraine", "Southern Europe", "Western Europe", "Scandinavia", "Great Britain"}, 480, 160, 80, 60) },
		{"Western Europe", Territory("Western Europe", {"Northern Europe", "Southern Europe", "North Africa", "Great Britain"}, 400, 240, 40, 80) },
		{"Southern Europe", Territory("Southern Europe", {"Northern Europe", "Western Europe", "Egypt", "Middle East", "Ukraine"}, 440, 220, 160, 80) },

		{"North Africa", Territory("North Africa", {"Brazil", "Egypt", "East Africa", "Congo", "Western Europe"}, 400, 320, 80, 80) },
		{"Egypt", Territory("Egypt", {"North Africa", "East Africa", "Southern Europe", "Middle East"}, 480, 320, 80, 40) },
		{"East Africa", Territory("East Africa", {"North Africa", "Egypt", "Congo", "South Africa", "Madagascar", "Middle East"}, 480, 360, 80, 80) },
		{"Congo", Territory("Congo", {"East Africa", "South Africa", "North Africa"}, 440, 400, 40, 40) },
		{"South Africa", Territory("South Africa", {"East Africa", "Madagascar", "Congo"}, 440, 440, 80, 80) },
		{"Madagascar", Territory("Madagascar", {"East Africa", "South Africa"}, 600, 440, 40, 80) },

		{"Ural", Territory("Ural", {"Siberia", "China", "Afghanistan", "Ukraine"}, 680, 40, 80, 140) },
		{"Siberia", Territory("Siberia", {"China", "Mongolia", "Ural", "Irkutsk", "Yakutsk"}, 760, 40, 40, 120) },
		{"Yakutsk", Territory("Yakutsk", {"Irkutsk", "Siberia", "Kamchatka"}, 800, 40, 80, 40) },
		{"Irkutsk", Territory("Irkutsk", {"Yakutsk", "Siberia", "Kamchatka", "Mongolia"}, 800, 80, 80, 40) },
		{"Kamchatka", Territory("Kamchatka", {"Yakutsk", "Irkutsk", "Mongolia", "Alaska", "Japan"}, 880, 40, 40, 80) },
		{"Japan", Territory("Japan", {"Kamchatka", "Mongolia"}, 960, 80, 40, 120) },
		{"Siam", Territory("Siam", {"China", "India", "Indonesia"}, 760, 320, 120, 40) },
		{"China", Territory("China", {"Afghanistan", "India", "Siam", "Ural", "Siberia", "Mongolia"}, 760, 160, 120, 160) },
		{"Mongolia", Territory("Mongolia", {"China", "Irkutsk", "Kamchatka", "Japan", "Siberia"}, 800, 120, 120, 40) },
		{"Afghanistan", Territory("Afghanistan", {"Middle East", "Ukraine", "Ural", "India", "China"}, 680, 180, 80, 60) },
		{"India", Territory("India", {"Middle East", "Afghanistan", "Siam", "China"}, 680, 240, 80, 120) },
		{"Middle East", Territory("Middle East", {"Egypt", "East Africa", "India", "Southern Europe", "Ukraine", "Afghanistan"}, 600, 220, 80, 140) },

		{"Indonesia", Territory("Indonesia", {"Siam", "New Guinea", "Western Australia"}, 840, 400, 80, 80) },
		{"New Guinea", Territory("New Guinea", {"Indonesia", "Western Australia", "Eastern Australia"}, 960, 400, 40, 40) },
		{"Eastern Australia", Territory("Eastern Australia", {"Western Australia", "New Guinea"}, 960, 520, 80, 75) },
		{"Western Australia", Territory("Western Australia", {"Indonesia", "New Guinea", "Eastern Australia"}, 880, 520, 80, 75) },
	};
	continents = {
		{"North Amercia", {"Alaska", "Northwest Territory", "Greenland", "Alberta", "Ontario", "Quebec", "Central America", "Eastern United States", "Western United States"}},
		{"South America", {"Venezuela", "Peru", "Brazil", "Argentina"}},
		{"Africa", {"North Africa", "Egypt", "East Africa", "Congo", "South Africa", "Madagascar"}},
		{"Europe", {"Iceland", "Scandinavia", "Ukraine", "Great Britain", "Northern Europe", "Southern Europe", "Western Europe"}},
		{"Asia", {"Ural", "Siberia", "Yakutsk", "Irkutsk", "Kamchatka", "Mongolia", "Japan", "China", "India", "Afghanistan", "Middle East", "Siam"}},
		{"Australia", {"Indonesia", "New Guinea", "Western Australia", "Eastern Australia"}}
	};
	contExt = {
		{"North Amercia", 5},
		{"South America", 2},
		{"Africa", 3},
		{"Europe", 5},
		{"Asia", 7},
		{"Australia", 2}
	};
	lines = {
		{77,280,203,280},
		{80,197,80,563},
		{77,560,163,560},
		{160,563,160,437},
		{157,440,243,440},
		{240,317,240,443},
		{200,323,200,277},
		{197,320,243,320},
		{37,200,83,200},
		{40,203,40,37},
		{203,40,37,40},
		{280,117,280,223},
		{117,220,283,220},
		{120,217,120,283},
		{237,5,323,5},
		{320,83,320,2},
		{437,520,523,520},
		{560,443,560,317},
		{397,320,563,320},
		{400,317,400,403},
		{397,400,443,400},
		{440,523,440,397},
		{597,440,643,440},
		{640,523,640,437},
		{643,520,597,520},
		{837,400,923,400},
		{840,397,840,483},
		{957,400,1003,400},
		{1000,443,1000,397},
		{1040,517,1040,595},
		{877,592,1043,592},
		{880,595,880,517},
		{443,40,397,40},
		{400,83,400,37},
		{400,117,400,203},
		{400,237,400,323},
		{443,320,397,320},
		{440,323,440,297},
		{603,300,437,300},
		{600,303,600,217},
		{683,220,597,220},
		{680,223,680,37},
		{477,40,683,40},
		{677,40,923,40},
		{920,83,920,37},
		{1003,80,957,80},
		{1000,77,1000,203},
		{957,200,1003,200},
		{960,203,960,157},
		{877,160,923,160},
		{880,157,880,363},
		{597,360,883,360},
		{600,363,600,217},
		{683,220,597,220},
		{680,223,680,37}
	};
	dlines = {
		{200,40,240,40},
		{200,100,240,80},
		{280,80,280,120},
		{320,40,400,60},
		{440,60,480,80},
		{420,80,420,120},
		{920,50,1200,50},
		{0,50,40,50},
		{440,140,480,110},
		{500,120,500,160},
		{440,180,480,180},
		{420,200,420,240},
		{920,100,960,100},
		{920,140,960,140},
		{240,360,400,360},
		{560,400,600,440},
		{520,480,600,480},
		{840,360,860,400},
		{920,420,960,420},
		{900,480,900,520},
		{940,520,960,440},
		{1000,440,1000,520}
	};
	inventory = {
		{},
		{},
		{},
		{}
	};
	buttons = {
		{"start classic game",{"     Start\n\n\n\n Classic Game",{100,300,400,300},50,YELLOW,BLACK}},
		{"start blitz game",{"     Start\n\n\n\n   Blitz Game",{700,300,400,300},50,YELLOW,BLACK}},
		{"exit game", {"EXIT", {500,700,200,100}, 60,YELLOW, BLACK}},
		{"UseCard end phase", {"End Phase", {900, 800, 200, 50},30,YELLOW, BLACK}},
		{"AttTer end phase", {"End Phase", {900, 800, 200, 50},30,YELLOW, BLACK}},
		{"AttTar cancel attack", {"Cancel Attack", {800, 800, 300, 50},30,YELLOW, BLACK}},
		{"ForTer end phase", {"End Phase", {900, 800, 200, 50},30,YELLOW, BLACK}},
		{"Fortify+", {"+", {250, 805, 40, 40},20,WHITE, BLACK}},
		{"Fortify-", {"-", {0, 805, 40, 40},20,WHITE, BLACK}},
		{"ForTar cancel fortify", {"Cancel Fortify", {800, 800, 300, 50},30,YELLOW, BLACK}},
		{"ForCount cancel fortify", {"Cancel Fortify", {800, 800, 300, 50},30,YELLOW, BLACK}},
	};
	names = {
	"Alaska", "Northwest Territory", "Greenland", "Alberta", "Ontario",
	"Quebec", "Western United States", "Eastern United States", "Central America",
	"Venezuela", "Peru", "Brazil", "Argentina", "North Africa", "Egypt",
	"East Africa", "Congo", "South Africa", "Madagascar", "Middle East",
	"Afghanistan", "India", "Siam", "China", "Siberia", "Ural", "Yakutsk",
	"Irkutsk", "Mongolia", "Kamchatka", "Japan", "Indonesia", "New Guinea",
	"Western Australia", "Eastern Australia", "Ukraine", "Northern Europe",
	"Southern Europe", "Western Europe", "Scandinavia", "Great Britain", "Iceland"
	};
	cardStars = {
		{"Alaska", 1},
	{"Northwest Territory", 2},
	{"Greenland", 1},
	{"Alberta", 1},
	{"Ontario", 1},
	{"Quebec", 1},
	{"Western United States", 1},
	{"Eastern United States", 2},
	{"Central America", 1},
	{"Venezuela", 2},
	{"Peru", 1},
	{"Brazil", 2},
	{"Argentina", 1},
	{"North Africa", 1},
	{"Egypt", 1},
	{"East Africa", 2},
	{"Congo", 1},
	{"South Africa", 2},
	{"Madagascar", 1},
	{"Middle East", 2},
	{"Afghanistan", 1},
	{"India", 1},
	{"Siam", 2},
	{"China", 1},
	{"Siberia", 1},
	{"Ural", 1},
	{"Yakutsk", 1},
	{"Irkutsk", 1},
	{"Mongolia", 2},
	{"Kamchatka", 1},
	{"Japan", 1},
	{"Indonesia", 1},
	{"New Guinea", 1},
	{"Western Australia", 2},
	{"Eastern Australia", 1},
	{"Ukraine", 2},
	{"Northern Europe", 1},
	{"Southern Europe", 1},
	{"Western Europe", 1},
	{"Scandinavia", 2},
	{"Great Britain", 1},
	{"Iceland", 1}
	};
	deck = {
	{"Alaska", 1},
	{"Northwest Territory", 2},
	{"Greenland", 1},
	{"Alberta", 1},
	{"Ontario", 1},
	{"Quebec", 1},
	{"Western United States", 1},
	{"Eastern United States", 2},
	{"Central America", 1},
	{"Venezuela", 2},
	{"Peru", 1},
	{"Brazil", 2},
	{"Argentina", 1},
	{"North Africa", 1},
	{"Egypt", 1},
	{"East Africa", 2},
	{"Congo", 1},
	{"South Africa", 2},
	{"Madagascar", 1},
	{"Middle East", 2},
	{"Afghanistan", 1},
	{"India", 1},
	{"Siam", 2},
	{"China", 1},
	{"Siberia", 1},
	{"Ural", 1},
	{"Yakutsk", 1},
	{"Irkutsk", 1},
	{"Mongolia", 2},
	{"Kamchatka", 1},
	{"Japan", 1},
	{"Indonesia", 1},
	{"New Guinea", 1},
	{"Western Australia", 2},
	{"Eastern Australia", 1},
	{"Ukraine", 2},
	{"Northern Europe", 1},
	{"Southern Europe", 1},
	{"Western Europe", 1},
	{"Scandinavia", 2},
	{"Great Britain", 1},
	{"Iceland", 1}
	};
	visited = {};
}



void Game::draw() {
	switch (gamePhase) {
	case Menu: drawMenu(); break;
	case PreGame: drawPreGame(); break;
	case UseCard: drawUseCard(); break;
	case PlaceTroops: drawPlaceTroops(); break;
	case AttackChooseTer: drawAttackChooseTer(); break;
	case AttackChooseTar: drawAttackChooseTar(); break;
	case FortifyChooseTer: drawFortifyChooseTer(); break;
	case FortifyChooseCount: drawFortifyChooseCount(); break;
	case FortifyChooseTar: drawFortifyChooseTar(); break;
	}
}

void Game::drawMenu() {
	drawButton("start classic game");
	drawButton("start blitz game");
	drawButton("exit game");
}

void Game::drawPreGame() {
	DrawText("Game is starting", 50, 300, 135, BLACK);
}

void Game::drawUseCard() {
	drawMap();
	DrawText("Current Phase: Use Cards", 20, 660, 30, BLACK);
	drawCards();
	drawButton("UseCard end phase");
}

void Game::drawPlaceTroops() {
	drawMap();
	DrawText("Current Phase: Place Troops", 20, 660, 30, BLACK);
	drawTroopsToPlace();
}

void Game::drawAttackChooseTer() {
	drawMap();
	DrawText("Current Phase: Choose Attacking Territory", 20, 660, 30, BLACK);
	drawButton("AttTer end phase");
}

void Game::drawAttackChooseTar() {
	drawMap();
	DrawText("Current Phase: Choose Attack Target", 20, 660, 30, BLACK);
	drawAttacker();
	drawButton("AttTar cancel attack");
}

void Game::drawFortifyChooseTer() {
	drawMap();
	DrawText("Current Phase: Choose Fortify Starting Territory", 20, 660, 30, BLACK);
	drawButton("ForTer end phase");
}

void Game::drawFortifyChooseCount() {
	drawMap();
	DrawText("Current Phase: Choose Fortify Count", 20, 660, 30, BLACK);
	drawButton("Fortify+");
	drawButton("Fortify-");
	drawButton("ForCount cancel fortify");
	DrawRectangle(40, 800, 200, 50, YELLOW);
	DrawText(to_string(forCount).c_str(), 130, 810, 30, BLACK);
}

void Game::drawFortifyChooseTar() {
	drawMap();
	DrawText("Current Phase: Choose Fortify Target Territory", 20, 660, 30, BLACK);
	drawAttacker();
}

void Game::drawMap() {
	drawTerritories();
	drawLines();
	drawSL();
	drawTroops();
	drawName();
	drawContExts();
	drawCurrentPlayer();
}

void Game::drawTerritories() {
	for (auto& [n, t] : territory) {
		DrawRectangle(t.x, t.y, t.w, t.h, pColor[t.owner]);
		DrawRectangleLinesEx({ t.x - 1, t.y - 1, t.w + 2, t.h + 2 }, 2, BLACK);
	}
}

void Game::drawLines() {
	for (Vector4 line : dlines) {
		drawDashedLine(line.x, line.y, line.z, line.w, 4, 10, BLACK);
	}
	for (Vector4 line : lines) {
		DrawLineEx({ line.x, line.y  }, { line.z , line.w }, 6, BLACK);
	}
}

void Game::drawSL() {
	DrawLineEx({ 0,600 }, { 1200,600 }, 4, BLACK);
	DrawRectangle(0, 602, 1200, 298, BROWN);
}

void Game::drawTroops() {
	for (auto& [n, t] : territory) {
		DrawText(to_string(t.troops).c_str(), t.x + 4, t.y + 4, 20, BLACK);
	}
}

void Game::drawName() {
	for (auto& [n, t] : territory) {
		if (n == "Eastern Australia")
		{
			DrawText(t.name.c_str(), t.x + 4, t.y + 35, 10, BLACK);
			continue;
		}
		DrawText(t.name.c_str(), t.x + 4, t.y + 25, 10, BLACK);
	}
}

void Game::drawDashedLine(float x, float y, float z, float w, float thickness, float dashLength, Color color) {
	float lineLength = Vector2Distance({ x, y }, { z, w });
	float angle = atan2f(w - y, z - x);
	int numDashes = (int)(lineLength / dashLength);

	for (int i = 0; i < numDashes; ++i) {
		if (i % 2 == 0) continue;
		float startX = x + i * dashLength * cosf(angle);
		float startY = y + i * dashLength * sinf(angle);
		float endX = startX + dashLength * cosf(angle);
		float endY = startY + dashLength * sinf(angle);

		DrawLineEx( { startX, startY }, { endX, endY }, thickness, color);
	}
}

void Game::drawButton(string btext) {
	Button button = buttons[btext];
	DrawRectangleRec(button.buttonRec, button.color);
	DrawRectangleLinesEx(button.buttonRec, 6, BLACK);
	DrawText(button.buttonText.c_str(), static_cast<int>(button.textPos.x) - MeasureText(button.buttonText.c_str(), button.fontSize) / 2, static_cast<int>(button.textPos.y) - button.fontSize / 2, button.fontSize, button.textColor);
}

void Game::drawContExts() {
	int i = 80;
	for (auto &[n, w] : contExt) {
		string str = n + ": " + to_string(w);
		DrawText(str.c_str(), 1020, i, 20, BLACK);
		i += 40;
	}
}

void Game::drawCurrentPlayer() {
	DrawRectangle(270, 610, 130, 50, pColor[currentPlayer]);
	string str = "Current Player: Player" + to_string(currentPlayer);
	DrawText(str.c_str(), 20, 620, 30, BLACK);
}

void Game::drawCards() {
	float i = 700;
	int j = 1;
	for (auto& card : inventory[currentPlayer]) {
		if (j <= 3) {
			buttons[card.cardName] = Button(card.cardName + ": " + to_string(card.cardStars), { 20,i,300,50 }, 20, WHITE, BLACK);
			drawButton(card.cardName);
			i += 50;
		}
		else {
			buttons[card.cardName] = Button(card.cardName + ": " + to_string(card.cardStars), { 330,i-150,300,50 }, 20, WHITE, BLACK);
			drawButton(card.cardName);
			i += 50;
		}
		j++;
	}
	cardsDrawn = true;
}

void Game::drawTroopsToPlace() {
	DrawRectangle(800, 800, 400, 50, YELLOW);
	string str = "Troops Remaining: " + to_string(currentPlayerTroops);
	DrawText(str.c_str(), 820, 810, 30, BLACK);
}

void Game::drawAttacker() {
	string str = "Attacker: " + attTer;
	DrawText(str.c_str(), 20, 700, 30, BLACK);
}

void Game::drawDefender() {
	string str = "Defender: " + attTar;
	DrawText(str.c_str(), 20, 740, 30, BLACK);
}



//-----------------------------------------------------------------------------




void Game::update() {
	if (WindowShouldClose())
		running = false;
	switch (gamePhase) {
	case Menu: updateMenu(); break;
	case PreGame: updatePreGame(); break;
	case UseCard: updateUseCard(); break;
	case PlaceTroops: updatePlaceTroops(); break;
	case AttackChooseTer: updateAttackChooseTer(); break;
	case AttackChooseTar: updateAttackChooseTar(); break;
	case FortifyChooseTer: updateFortifyChooseTer(); break;
	case FortifyChooseCount: updateFortifyChooseCount(); break;
	case FortifyChooseTar: updateFortifyChooseTar(); break;
	case ChangePlayer: nextPlayer(); break;
		}
}

void Game::updateMenu() {
	if (checkButton("start classic game"))
		gamePhase = PreGame;
	if (checkButton("start blitz game")) {
		blitz = true;
		gamePhase = PreGame;
	}
	if (checkButton("exit game"))
		running = false;
}

void Game::updatePreGame() {
	if (waited3s) {
		waited3s = false;
		gamePhase = UseCard;
	}
	else {
		giveTerritories();
		shuffleDeck();
		WaitTime(1.0);
		waited3s = true;
	}
}

void Game::updateUseCard() {
	if (!isAlive[currentPlayer]) {
		nextPlayer();
		return;
	}
	for (int i = 1; i < playerCount; i++) {
		playerTerritoryCount = 0;
		for (auto &[n, t] : territory)
			if (t.owner == i)
				playerTerritoryCount++;
		if (playerTerritoryCount == 0)
			isAlive[i] = false;
	}


	currentPlayerTroops = 0;
	if (inventory[currentPlayer].size() == 0) {
		giveTroops();
		giveCardTroops();
		cardsDrawn = false;
		gamePhase = PlaceTroops;
		return;
	}
	if (checkButton("UseCard end phase")) {
		giveTroops();
		giveCardTroops();
		cardsDrawn = false;
		gamePhase = PlaceTroops;
		return;
	}
	int i = 0;
	for (auto &card : inventory[currentPlayer]) {
		if (checkButton(card.cardName)) {
			usedCardsStars += card.cardStars;
			deck.push_back( inventory[currentPlayer][i]);
			inventory[currentPlayer].erase(inventory[currentPlayer].begin()+i);
		}
		i++;
	}
}

void Game::updatePlaceTroops() {
	checkTerClicked();
	if (isTerClicked && currentPlayerTroops > 0 && territory[terClicked].owner == currentPlayer) {
		territory[terClicked].troops++;
		currentPlayerTroops--;
		isTerClicked = false;
		terClicked = "";
	}
	if (currentPlayerTroops == 0) {
		gamePhase = AttackChooseTer;
	}
}

void Game::updateAttackChooseTer() {
	checkTerClicked();
	if (isTerClicked && territory[terClicked].owner == currentPlayer && terClickedHasBorder()) {
		attTer = terClicked;
		terClicked = "";
		isTerClicked = false;
		gamePhase = AttackChooseTar;
	}
	if (checkButton("AttTer end phase")) {
		if (conquered1) {
			getCard();
			conquered1 = false;
		}
		gamePhase = FortifyChooseTer;
	}
}

void Game::updateAttackChooseTar() {
	checkTerClicked();
	if (isTerClicked && territory[terClicked].owner != currentPlayer && isNeighbor()) {
		attTar = terClicked;
		terClicked = "";
		isTerClicked = false;
		attack();
		attTar = "";
		attTer = "";
		gamePhase = AttackChooseTer;
	}
	if (checkButton("AttTar cancel attack")) {
		gamePhase = AttackChooseTer;
		attTer = "";
	}
}

void Game::updateFortifyChooseTer() {
	checkTerClicked();
	visited.clear();
	if (isTerClicked && territory[terClicked].owner == currentPlayer && hasAnyPath(terClicked) && territory[terClicked].troops > 1) {
		attTer = terClicked;
		terClicked = "";
		isTerClicked = false;
		gamePhase = FortifyChooseCount;
	}
	if (checkButton("ForTer end phase")) {
		gamePhase = ChangePlayer;
	}
}

void Game::updateFortifyChooseCount() {
	if (CheckCollisionPointRec(GetMousePosition(), { 40, 800, 200, 50 }) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
		gamePhase = FortifyChooseTar;

	if (checkButton("Fortify+") && forCount < territory[attTer].troops - 1)
		forCount++;

	if (checkButton("Fortify-") && forCount > 1)
		forCount--;

	if (checkButton("ForCount cancel fortify")) {
		gamePhase = FortifyChooseTer;
		forCount = 1;
		attTer = "";
	}
}

void Game::updateFortifyChooseTar() {
	checkTerClicked();
	visited.clear();
	if (isTerClicked && territory[terClicked].owner == currentPlayer && hasPath(attTer, terClicked) && terClicked != attTer) {
		attTar = terClicked;
		territory[attTar].troops += forCount;
		territory[attTer].troops -= forCount;
		forCount = 1;
		terClicked = "";
		isTerClicked = false;
		attTar = "";
		attTer = "";
		gamePhase = ChangePlayer;
	}

	if (checkButton("ForTar cancel fortify")) {
		gamePhase = FortifyChooseTer;
		forCount = 1;
		attTer = "";
		attTar = "";
	}
}

int Game::nextPlayer() {
	
	usedCardsStars = 0;
	currentPlayer++;
	if (currentPlayer > playerCount)
		currentPlayer = 1;
	gamePhase = UseCard;
	return currentPlayer;
}

bool Game::checkButton(string btext) {
	Button button = buttons[btext];
	return (CheckCollisionPointRec(GetMousePosition(), button.buttonRec) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT));
}

void Game::shuffleNames() {
	auto rng = std::default_random_engine(random_device{}());
	ranges::shuffle(names, rng);
}

void Game::shuffleDeck() {
	auto rng = std::default_random_engine{};
	ranges::shuffle(deck, rng);
}

void Game::giveTerritories() {
	shuffleNames();
	for (int i = 0; i < territoryCount; i++) {
		territory[names[i]].owner = i % 3 + 1;
		territory[names[i]].troops = cardStars[names[i]];
	}
}

void Game::giveTroops() {
	int pTerCount = 0;
	for (auto &[n, t] : territory) 
		if (t.owner == currentPlayer)
			pTerCount++;
	currentPlayerTroops += pTerCount / 3;

}

void Game::checkTerClicked() {
	for (auto &[n, t] : territory) {
		if (CheckCollisionPointRec(GetMousePosition(), { t.x, t.y, t.w, t.h }) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			terClicked = n;
			isTerClicked = true;
		}
	}
}

void Game::giveCardTroops() {
	switch (usedCardsStars) {
	case 1:
		currentPlayerTroops += 1;
		break;
	case 2:
		currentPlayerTroops += 2;
		break;
	case 3:
		currentPlayerTroops += 4;
		break;
	case 4:
		currentPlayerTroops += 7;
		break;
	case 5:
		currentPlayerTroops += 10;
		break;
	case 6:
		currentPlayerTroops += 13;
		break;
	case 7:
		currentPlayerTroops += 17;
		break;
	case 8:
		currentPlayerTroops += 21;
		break;
	case 9:
		currentPlayerTroops += 25;
		break;
	case 10:
		currentPlayerTroops += 30;
		break;
	}
}

bool Game::terClickedHasBorder() {
	for (string nb : territory[terClicked].neighbors) 
		if (territory[nb].owner != currentPlayer)
			return true;
	return false;
}

bool Game::isNeighbor() {
	for (string t : territory[attTer].neighbors) {
		if (t == terClicked)
			return true;
	}
	return false;
}

void Game::attack() {
	if (blitz) {
		while (territory[attTer].troops > 1 && territory[attTar].owner != currentPlayer) {
			attackRound();
		}
	}
	else {
		attackRound();
	}
}

void Game::attackRound() {
	int attackers = territory[attTer].troops - 1;
	if (attackers == 0) {
		gamePhase = AttackChooseTer;
		attTer = "";
		attTar = "";
		return;
	}
	int defenders = territory[attTar].troops;


	int attackDice = min(attackers, 3);
	int defendDice = min(defenders, 2);


	vector<int> attRolls(attackDice);
	vector<int> defRolls(defendDice);
	for (int i = 0; i < attackDice; ++i)
		attRolls[i] = rand() % 6 + 1;
	for (int i = 0; i < defendDice; ++i)
		defRolls[i] = rand() % 6 + 1;

	sort(attRolls.rbegin(), attRolls.rend());
	sort(defRolls.rbegin(), defRolls.rend());

	// Compare dice rolls
	int attLosses = 0, defLosses = 0;
	for (int i = 0; i < std::min(attackDice, defendDice); ++i) {
		if (attRolls[i] > defRolls[i])
			defLosses++;
		else
			attLosses++;
	}

	// Apply losses
	territory[attTer].troops -= attLosses;
	territory[attTar].troops -= defLosses;

	// Check if territory is conquered
	if (territory[attTar].troops <= 0) {
		territory[attTar].owner = territory[attTer].owner;
		territory[attTar].troops = attackers - attLosses;
		territory[attTer].troops -= attackers - attLosses;
		conquered1 = true;
	}
}

void Game::getCard() {
	if (!deck.empty() && inventory[currentPlayer].size()<5) {
		inventory[currentPlayer].push_back(deck[0]);
		deck.erase(deck.begin());
	}
}

bool Game::hasPath(string start, string end) {
	if (start == end) {
		return true; // Reached the destination.
	}

	visited.insert(start); // Mark the current territory as visited.

	int owner = territory[start].owner; // Get the owner of the start territory.

	for (const string& neighbor : territory[start].neighbors) {
		if (visited.find(neighbor) == visited.end() && territory[neighbor].owner == owner) {
			if (hasPath(neighbor, end)) {
				return true; // A valid path exists.
			}
		}
	}

	return false; // No valid path found.
}

bool Game::hasAnyPath(string start) {
	int owner = territory[start].owner; // Get the owner of the start territory.

	for (const string& neighbor : territory[start].neighbors) {
		if (territory[neighbor].owner == owner) {
			visited.clear(); // Clear the visited set for each new pathfinding attempt.
			if (hasPath(start, neighbor)) {
				return true; // If any path exists to a neighbor, return true.
			}
		}
	}

	return false; // No path found to any other owned territory.
}