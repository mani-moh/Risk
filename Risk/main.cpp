#include "Risk.h"
using namespace std;

int main()
{
	//init -----------------------------------------------------------------------------
	Game game ;
	InitWindow(1200, 900, "Risk");
	SetTargetFPS(60);

	//game loop ------------------------------------------------------------------------
		while(game.running)
		{
			//update
			game.update();

			//draw
			BeginDrawing();
			ClearBackground(game.bgColor);
			game.draw();
			EndDrawing();
		}
	CloseWindow();
	return 0;
}