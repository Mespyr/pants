#include <cstdint>
#include <iostream>
#include <SDL2/SDL.h>

#include "include/util.h"
#include "include/engine.h"
#include "include/object.h"

int32_t main() {
	if (init_lib() != 0)
		return 1;

	////////////////// initialize engine and textures ///////////////////
	Engine engine;
	if (engine.init_window() != 0)
		return 1;

	SDL_Texture* player_texture = engine.load_texture("assets/player.png");
	engine.init_textures(player_texture);
	/////////////////////////////////////////////////////////////////////

	engine.player.player_object.position = {100, 100}; // temporarily set player position
	
	bool quit = false;
	SDL_Event e;
	uint32_t now, last_game_step = SDL_GetTicks();
	float_t delta_time;

	// create clone to show how movement of player changes the position of stuff on the screen
	Object clone(20, 20, player_texture);
	clone.current_frame.x = 0;
	clone.current_frame.y = 0;
	clone.current_frame.w = 16;
	clone.current_frame.h = 16;
	Object clone2(50, 50, player_texture);
	clone2.current_frame.x = 16;
	clone2.current_frame.y = 0;
	clone2.current_frame.w = 16;
	clone2.current_frame.h = 16;

	// game loop
	while (!quit) {
		now = SDL_GetTicks();

		while (SDL_PollEvent(&e))
			if (e.type == SDL_QUIT)
				quit = true;

		if (last_game_step < now) {
			delta_time = (now - last_game_step) / 1000.0f;
			engine.clear();

			engine.render_object(clone);
			engine.render_object(clone2);

			engine.render_player();
			engine.display();
			// move player to show that objects will move relative to the player
			engine.player.player_object.position.x -= delta_time * 100;
		}
		else SDL_Delay(1);
		last_game_step = now;
	}
	
	SDL_Quit();
	return 0;
}