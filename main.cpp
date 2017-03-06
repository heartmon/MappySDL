#define _CRT_SECURE_NO_WARNINGS

#include "stdio.h"
#include "avancezlib.h"

const unsigned int	NUM_LIVES = 2;

const unsigned int	MAX_NUM_ROCKETS = 32;
const unsigned int	MAX_NUM_BOMBS = 32;
const unsigned int	POINTS_PER_ALIEN = 100;
const float			FIRE_TIME_INTERVAL = .5f;
const float			BOMB_TIME_INTERVAL = 1.25f;
const float			PLAYER_SPEED = 160.0f;
const float			ROCKET_SPEED = 160.0f;
const float			ALIEN_SPEED = 40.0f;
const float			BOMB_SPEED = 120.0f;

bool game_over = false;
float game_speed = 1.f;		// speed of the game; it is increased each time all the aliens are hit
							// it is also the score multiplier

float gravity = 0.2f;
float maxFallSpeed = -5.0f;

#include "game.h"
#include "global_constant.h"
#include "start_screen.h"
#include "router.h"
#include "game_over_screen.h"

int num_frames = 0;
int sum_delta = 0;
float avg_fps = 0;
char msg[256];

int main(int argc, char** argv)
{

	AvancezLib system;

	Router router;
	router.setCurrentScreen(START_SCREEN);

	GameViewport gameViewport;
	gameViewport.Init();

	system.init(WINDOW_WIDTH, WINDOW_HEIGHT, &router, &gameViewport);


	StartScreen startScreen;
	startScreen.Create(&system);
	startScreen.Init();

	GameOverScreen gameOverScreen;
	gameOverScreen.Create(&system);
	gameOverScreen.Init();

	Game game;
	game.Create(&system);
	game.Init();

	new GameViewport();

	int currentScreen = 0;

	float lastTime = system.getElapsedTime();
	while (system.update())
	{
		float newTime = system.getElapsedTime();
		float dt = newTime - lastTime;
		dt = dt * game_speed;
		lastTime = newTime;

		sum_delta += dt;
		//SDL_Log("%f", dt);
	

		//num_frames++;

		//if (sum_delta > 100)
		//{
		//	avg_fps = ((float)num_frames / sum_delta) * 1000;
		//	num_frames = 0;
		//	sum_delta = 0;
		//}
		//sprintf(msg, "%.3f fps", avg_fps);
		//system.drawText(12, 12, msg);
		if (router.getCurrentScreen() == START_SCREEN) {
			startScreen.Update(dt);
			startScreen.Draw();
		}

		if (router.getCurrentScreen() == PLAY_SCREEN) {
			if (!game.isGameOver()) {
				game.Update(dt);
			}
			else {
				router.setCurrentScreen(GAME_OVER_SCREEN);
			}
		}

		if (router.getCurrentScreen() == GAME_OVER_SCREEN) {
			gameOverScreen.Update(dt);
			gameOverScreen.Draw();
		}

		//game.Draw();
	}

	// clean up
	game.Destroy();
	system.destroy();

	return 0;
}

//int main(int argc, char** argv)
//{
//	AvancezLib system;
//
//	system.init(640, 480);
//
//	Mouse mouse;
//	//MouseRenderComponent mouseRenderComponent = new MouseRenderComponent(&system);
//	mouse.Init( &system, new MouseRenderComponent(&system), new MouseInputComponent(&system) );
//
//
//	float lastTime = system.getElapsedTime();
//	while (system.update())
//	{
//		//Time
//		float newTime = system.getElapsedTime();
//		float dt = newTime - lastTime;
//		
//		dt = dt * game_speed;
//		lastTime = newTime;
//
//		//Update
//		mouse.Update(dt);
//
//		if (game_over)
//			dt = 0.f;
//
//		if (game_over)
//		{
//			//sprintf(msg, "*** G A M E  O V E R ***");
//			//system.drawText(250, 8, msg);
//		}
//	}
//
//	//aliens.destroy();
//	//player.destroy();
//	system.destroy();
//
//	return 0;
//}



