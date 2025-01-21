#pragma once
#include <raylib.h>
#include <raymath.h>
#include <string>
#include <vector>
#include <map>
#include <unordered_set>
using namespace std;


enum GamePhase {
	Menu,PreGame,UseCard,GetTroops,PlaceTroops,AttackChooseTer,AttackChooseTar,AttackChoosetype,FortifyChooseTer,FortifyChooseCount,FortifyChooseTar,ChangePlayer
};
class Game;
struct Territory;
struct Card;
struct Button;

class Game {
public:
	bool running;
	bool waited3s;
	bool conquered1;
	bool cardsDrawn;
	int usedCardsStars;
	int currentPlayerTroops;
	Color bgColor;
	Color pColor[4];
	bool isAlive[4];
	int playerCount;
	int territoryCount;
	int currentPlayer;
	bool isTerClicked;
	string terClicked;
	string attTer;
	string attTar;
	bool blitz;
	int forCount;
	int playerTerritoryCount;
	GamePhase gamePhase;
	map<string,Territory> territory;
	vector<Vector4> dlines;
	vector<Vector4> lines;
	vector<string> names;
	vector<Card> deck;
	map<string, vector<string>> continents;
	map<string, int> contExt;
	vector<vector<Card>> inventory;
	map<string,Button> buttons;
	unordered_set<string> visited;
	map<string, int> cardStars;

	
	Game();
	void draw();
	void drawMap();
	void drawTerritories();
	void drawLines();
	void drawSL();
	void drawTroops();
	void drawName();
	void drawContExts();
	void drawCurrentPlayer();
	void drawCards();
	void drawTroopsToPlace();
	void drawAttacker();
	void drawDefender();
	void drawMenu();
	void drawPreGame();
	void drawUseCard();
	void drawPlaceTroops();
	void drawAttackChooseTer();
	void drawAttackChooseTar();
	void drawFortifyChooseTer();
	void drawFortifyChooseTar();
	void drawFortifyChooseCount();
	void attack();
	void attackRound();
	int nextPlayer();
	void update();
	void drawDashedLine(float x, float y, float z, float w, float thickness, float dashLength, Color color);
	void drawButton(string btext);
	bool checkButton(string btext);
	void shuffleNames();
	void shuffleDeck();
	void giveTerritories();
	void giveTroops();
	void checkTerClicked();
	void giveCardTroops();
	bool terClickedHasBorder();
	bool isNeighbor();
	void getCard();
	bool hasPath(string start, string end);
	bool hasAnyPath(string start);
	void updateMenu();
	void updatePreGame();
	void updateUseCard();
	void updatePlaceTroops();
	void updateAttackChooseTer();
	void updateAttackChooseTar();
	void updateFortifyChooseTer();
	void updateFortifyChooseTar();
	void updateFortifyChooseCount();
};

struct Territory {
	string name;
	vector<string> neighbors;
	int troops;
	float x, y, w, h;
	int owner;
	Territory() = default;
	Territory(string n, vector<string> nb, float xf, float yf, float wf, float hf);
};

struct Card {
	string cardName;
	int cardStars;
};

struct Button {
	string buttonText;
	Rectangle buttonRec;
	Vector2 textPos;
	int fontSize;
	Color color;
	Color textColor;
	Button() = default;
	Button(string n, Rectangle r, int s, Color bg, Color t);
};