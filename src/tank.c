#include "SDL.h"
#include "stdlib.h"
#include "math.h"

#include "config.h"
#include "aux-math.h"
#include "graphics.h"
#include "player.h"
#include "bullet.h"
#include "bot.h"
#include "wall.h"

// video surface
SDL_Surface *screen;

// time values
int time, old_time, time_since_fps;

// data
Player plr;
Bullet bul[BULLET_MAX];
Bot bot[BOT_MAX];
Wall wal[WALL_MAX];

// print frame rate
void print_fps(void) {
	printf("FPS: %f\n", 1000.0/(float)(time - old_time));
}

///////// GAME /////////////
void do_game()
{

	// Reset timer
	old_time = time;

	// Ask SDL for the time in milliseconds
	time = SDL_GetTicks();

	// Print frame rate
	if (PRINT_FPS && (time_since_fps > 1000)) {
		print_fps();
		time_since_fps = 0;
	} else {
		time_since_fps += time - old_time;
	}

	// Work out how much game time has passed since the previous frame
	float dt = (float)(time - old_time)*GAME_SPEED;

	// Maybe create new bot ** TEMP **
	if (rand()/(float)RAND_MAX < 0.01*dt) {
		spawn_bot(bot, 
					 random_int(DISPLAY_WIDTH - PLAYER_BOUND_WIDTH) + PLAYER_BOUND_WIDTH,
					 random_int(DISPLAY_HEIGHT - PLAYER_BOUND_WIDTH) + PLAYER_BOUND_WIDTH,
					 random_angle());
	}

	// Handle game objects
	do_walls(wal);
	do_player(&plr, bul, wal, dt);
	do_bots(bot, &plr, bul, wal, dt);
	do_bullets(bul, dt);

	// check for collisions between various objects
	int bul_i, bot_i, wall_i;
	for (bul_i = 0; bul_i < BULLET_MAX; bul_i++)
	{
		if (bul[bul_i].ex)
		{
			// Check for player being hit by a bullet
			if (player_hit_by_bullet(&plr, &bul[bul_i]))
			{
				bullet_destroy(&bul[bul_i]);
				player_take_bullet(&plr, &bul[bul_i]);
			}


			// Check for bot being hit by a bullet
			for (bot_i = 0; bot_i < BOT_MAX; bot_i++)
			{
				if (bot_hit_by_bullet(&bot[bot_i], &bul[bul_i]))
				{
					bullet_destroy(&bul[bul_i]);
					bot_take_bullet(&bot[bot_i], &bul[bul_i]);
				}
			}
			
			// Check for wall being hit by a bullet
			for (wall_i = 0; wall_i < WALL_MAX; wall_i++)
			{
				if (wall_hit_by_bullet(&wal[wall_i], &bul[bul_i]))
				{
					bullet_destroy(&bul[bul_i]);
					wall_take_bullet(&wal[wall_i]);
				}
			}
		}
	}


}

// Initial game setup
void setup()
{
	setup_player(&plr);
	time = SDL_GetTicks();
	old_time = 0;
	time_since_fps = 0;


	// TEMP (make some walls)
	// <<
	int ws = 100;

	spawn_wall(wal, i_vector(500, 140-ws), i_vector(501, 140+ws));
	spawn_wall(wal, i_vector(500-ws, 140), i_vector(500+ws, 141));

	spawn_wall(wal, i_vector(500, 340-ws), i_vector(500, 340+ws));
	spawn_wall(wal, i_vector(500-ws, 340), i_vector(500+ws, 341));

	spawn_wall(wal, i_vector(140, 340-ws), i_vector(141, 340+ws));
	spawn_wall(wal, i_vector(140-ws, 340), i_vector(140+ws, 341));
	// >>
}

// Render the entire screen
// (in the right order)
void render()
{   
	// Lock video surface (if needed)
	if (SDL_MUSTLOCK(screen)) 
		if (SDL_LockSurface(screen) < 0) 
			return;

	// Draw game objects to the screen
	draw_walls(screen, wal);
	draw_player(screen, &plr);
	draw_bots(screen, bot);
	draw_bullets(screen, bul);

	// Health bar
	int y, x;
	x = DISPLAY_WIDTH - 10 - plr.max_hp;
	for (y = DISPLAY_HEIGHT - 20; y < DISPLAY_HEIGHT - 10; y++)
	{
		put_line(screen, x, y, x + plr.hp, y, HEALTH_COLOR_GOOD);
		put_line(screen, x + plr.hp, y, DISPLAY_WIDTH - 10, y, HEALTH_COLOR_BAD);
	}

	// Unlock screen (if needed)
	if (SDL_MUSTLOCK(screen)) 
		SDL_UnlockSurface(screen);

	// Tell SDL to update the whole screen
	SDL_UpdateRect(screen, 0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT);    
}

// Entry point
int main(int argc, char *argv[])
{
	// Initialize SDL's subsystems - in this case, only video.
	if ( SDL_Init(SDL_INIT_VIDEO) < 0 ) 
	{
		fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
		exit(1);
	}

	// Register SDL_Quit to be called at exit; makes sure things are
	// cleaned up when we quit.
	atexit(SDL_Quit);

	Uint32 maybe_fullscreen;
	maybe_fullscreen = USE_FULLSCREEN ? SDL_FULLSCREEN : 0;

	// Attempt to create a window.
	screen = SDL_SetVideoMode(DISPLAY_WIDTH, DISPLAY_HEIGHT, 32, SDL_SWSURFACE | maybe_fullscreen);

	// If we fail, return error.
	if ( screen == NULL ) {
		fprintf(stderr, "Unable to set DISPLAY_WIDTHxDISPLAY_HEIGHT video: %s\n", SDL_GetError());
		exit(1);
	}

	setup();

	// Main loop: loop forever.
	while (1)
	{

		// Fill background black
		SDL_FillRect(screen, 0, 0);

		// advance game
		do_game();

		// Render stuff
		render();

		// Poll for events, and handle the ones we care about.
		SDL_Event event;
		while (SDL_PollEvent(&event)) 
		{
			switch (event.type) 
			{
				case SDL_KEYDOWN:
					switch (event.key.keysym.sym)
					{
						case 119: // W
							plr.y_input += 1;
							break;

						case 115: // S
							plr.y_input -= 1;
							break;

						case 97: // A
							plr.x_input -= 1;
							break;

						case 100: // D
							plr.x_input += 1;
							break;

						case SDLK_RETURN:
							spawn_wall(wal, i_vector(DISPLAY_WIDTH/2, DISPLAY_WIDTH/2), i_vector(plr.face.x, plr.face.y));
							break;

						default:
							break;
					}
					break;

				case SDL_MOUSEMOTION:			/* Mouse moved */
					plr.face.x = event.motion.x;
					plr.face.y = event.motion.y;

					// keep mouse in window
					if (event.motion.x < PLAYER_BOUND_WIDTH) 
						SDL_WarpMouse(PLAYER_BOUND_WIDTH,event.motion.y);

					if (event.motion.x > DISPLAY_WIDTH - PLAYER_BOUND_WIDTH) 
						SDL_WarpMouse(DISPLAY_WIDTH - PLAYER_BOUND_WIDTH,event.motion.y);

					if (event.motion.y < PLAYER_BOUND_WIDTH) 
						SDL_WarpMouse(event.motion.x,PLAYER_BOUND_WIDTH);

					if (event.motion.y > DISPLAY_HEIGHT - PLAYER_BOUND_WIDTH) 
						SDL_WarpMouse(event.motion.x,DISPLAY_HEIGHT - PLAYER_BOUND_WIDTH);

					break;

				case SDL_MOUSEBUTTONDOWN:		/* Mouse button pressed */
					plr.fire = 1;
					break;

				case SDL_MOUSEBUTTONUP:		/* Mouse button released */
					plr.fire = 0;
					break;

				case SDL_KEYUP:
					switch (event.key.keysym.sym)
					{
						case 119: // W
							plr.y_input -= 1;
							break;

						case 115: // S
							plr.y_input += 1;
							break;

						case 97: // A
							plr.x_input += 1;
							break;

						case 100: // D
							plr.x_input -= 1;
							break;

						default:
							break;
					}

					// If escape is pressed, return (and thus, quit)
					if (event.key.keysym.sym == SDLK_ESCAPE)
						return 0;
					break;

				case SDL_QUIT:
					return(0);
			}
		}
	}
	return 0;
}
