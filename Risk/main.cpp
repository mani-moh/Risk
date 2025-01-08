#include <Risk.h>


using namespace std;

int main()
{
	//init -----------------------------------------------------------------------------
	Game game;
	game.init();
	SetTargetFPS(60);
	Vector2 mousePoint;
	string attname;
	bool isPressed;
	bool terchosen=false;
	int moveCount=1;
	int skippedframes = 0;
	
	//game loop ------------------------------------------------------------------------
	
		while(true)
		{
			//update
			mousePoint = GetMousePosition();
			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
				isPressed = true;
			else
				isPressed = false;
			if (game.phase.gamePhase=="placement")
				for (const auto& [name, t] : game.territories) {
					if (t.owner==game.phase.player && CheckCollisionPointRec(mousePoint, game.territoryRects[name]) && isPressed) {
						game.phase.player->placeTroops(game.territories[name], 1);
						break;
					}
					if (game.phase.player->troops == 0) {
						game.phase.gamePhase = "attack";
						terchosen = false;
						
						break;
					}
				}
			if (game.phase.gamePhase == "attack") {
				if (!terchosen) {
					for (const auto& [name1, t1] : game.territories)
						if (t1.owner == game.phase.player && CheckCollisionPointRec(mousePoint, game.territoryRects[name1]) && isPressed && t1.troops > 1) {
							terchosen = true;
							attname = t1.name;
							break;
						}
					if (CheckCollisionPointRec(mousePoint, { 40, 800, 200, 50 }) && isPressed) {
						game.phase.gamePhase = "fortify";
						terchosen = false;
						attname = "";
						moveCount = 0;
					}
				}
				if (terchosen) {
					for (const auto& [name2, t2] : game.territories) {
						if (t2.owner != game.phase.player && CheckCollisionPointRec(mousePoint, game.territoryRects[name2]) && isPressed && game.territories[attname].checkNeighbor(game.territories[name2])) {
							game.territories[attname].attack(game.territories[name2], game.territories[attname].troops - 1);
							terchosen = false;
							attname = "";
							break;
						}
					}
					if (CheckCollisionPointRec(mousePoint, { 40, 800, 200, 50 }) && isPressed) {
						terchosen = false;
					}
				}
			}
			if (game.phase.gamePhase == "fortify") {
				
				if (!terchosen) {
					if (CheckCollisionPointRec(mousePoint, { 10, 815, 20, 20 }) && isPressed && moveCount>1) {
						moveCount--;
					}
					if (CheckCollisionPointRec(mousePoint, { 250, 815, 20, 20 }) && isPressed)
						moveCount++;
					for (const auto& [name1, t1] : game.territories)
						if (t1.owner == game.phase.player && CheckCollisionPointRec(mousePoint, game.territoryRects[name1]) && isPressed && t1.troops > moveCount) {
							bool canmove = false;
							for (const auto& [name3, t3] : game.territories) {
								unordered_set<Territory*> visited;
								if (game.phase.player->hasPath(&game.territories[name1], &game.territories[name3], visited)) {
									canmove = true;
									break;
								}
							}
							if (canmove) {
								terchosen = true;
								attname = t1.name;
							}
							break;
						}
					if (CheckCollisionPointRec(mousePoint, { 300, 800, 200, 50 }) && isPressed) {
						game.phase.gamePhase = "placement";
						terchosen = false;
						attname = "";
						moveCount = 1;
						game.nextPlayer();
						game.phase.player->addTroops(&game);
					}
				}
				
				if (terchosen)
					for (const auto& [name2, t2] : game.territories) {
						unordered_set<Territory*> visited;
						if (t2.owner == game.phase.player && CheckCollisionPointRec(mousePoint, game.territoryRects[name2]) && isPressed && name2 != attname && game.phase.player->hasPath(&game.territories[attname], &game.territories[name2], visited)) {
							game.phase.player->moveTroops(game.territories[attname], game.territories[name2],moveCount);
							terchosen = false;
							attname = "";
							game.phase.gamePhase = "placement";
							game.nextPlayer();
							game.phase.player->addTroops(&game);
							moveCount = 1;
							break;
						}
					}
			}
		
			//draw
			BeginDrawing();
			ClearBackground(game.backgroundColor);
			for (const auto& [name, t] : game.territories)
				DrawRectangle(game.territoryRects[name].x, game.territoryRects[name].y, game.territoryRects[name].width, game.territoryRects[name].height, t.owner->color);
			for (const auto& [name, t] : game.territories) {
				DrawLineEx({ game.territoryRects[name].x-4, game.territoryRects[name].y - 2 }, { game.territoryRects[name].x + game.territoryRects[name].width, game.territoryRects[name].y - 2 }, 4, game.lineColor);
				DrawLineEx({ game.territoryRects[name].x, game.territoryRects[name].y + game.territoryRects[name].height - 2 }, { game.territoryRects[name].x + game.territoryRects[name].width, game.territoryRects[name].y + game.territoryRects[name].height - 2 }, 4, game.lineColor);
				DrawLineEx({ game.territoryRects[name].x-2, game.territoryRects[name].y }, { game.territoryRects[name].x - 2 , game.territoryRects[name].y + game.territoryRects[name].height}, 4, game.lineColor);
				DrawLineEx({ game.territoryRects[name].x + game.territoryRects[name].width - 2, game.territoryRects[name].y}, {game.territoryRects[name].x + game.territoryRects[name].width - 2 , game.territoryRects[name].y + game.territoryRects[name].height}, 4, game.lineColor);
				char str0[10];
				_itoa_s(t.troops, str0, 10);
				DrawText(str0, game.territoryRects[name].x + 4, game.territoryRects[name].y + 4, 20, game.lineColor);
			}
			char str1[20] = "Player: ";
			strcat_s(str1, game.phase.player->name.c_str());
			DrawText(str1, 40, 640, 30, game.lineColor);
			char str2[20] = "Phase: ";
			strcat_s(str2, game.phase.gamePhase.c_str());
			DrawText(str2 , 40, 680, 30, game.lineColor);
			if (game.phase.gamePhase == "placement") {
				char a[10];
				char str3[20] = "Place Troops: ";
				_itoa_s(game.phase.player->troops, a, 10);
				strcat_s(str3, a);
				DrawText(str3, 40, 720, 30, game.lineColor);
			}
			else if (game.phase.gamePhase == "attack") {
				if (terchosen) {
					DrawText("Choose a target", 40, 720, 30, game.lineColor);
					DrawRectangle(40, 800, 200, 50, YELLOW);
					DrawText("Cancel attack", 60, 810, 20, game.lineColor);
				}
				else {
					DrawText("Choose a territory", 40, 720, 30, game.lineColor);
					DrawRectangle(40, 800, 200, 50, YELLOW);
					DrawText("End Phase", 60, 810, 30, game.lineColor);
				}
			}
			else if (game.phase.gamePhase == "fortify") {
				if (!terchosen) {
					DrawRectangle(40, 800, 200, 50, YELLOW);
					DrawRectangle(10, 815, 20, 20, WHITE);
					DrawRectangle(250, 815, 20, 20, WHITE);
					char str4[10];
					DrawText(TextFormat("%d", moveCount), 130, 810, 30, game.lineColor);
					DrawText("-", 15, 815, 20, game.lineColor);
					DrawText("+", 255, 815, 20, game.lineColor);
					DrawRectangle(300, 800, 200, 50, YELLOW);
					DrawText("End Phase", 320, 810, 30, game.lineColor);
				}
				if (terchosen)
					DrawText("Choose a target", 40, 720, 30, game.lineColor);
				else {
					DrawText("Choose a territory", 40, 720, 30, game.lineColor);
					
				}
			}

			EndDrawing();
			if (WindowShouldClose()) 
				break;
		}
	
	
	CloseWindow();
	return 0;
}