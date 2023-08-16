#pragma once

#include <cstdint>
#include "object.h"
#include "vec2.h"

class Player {
public:
	Player() {}

	void init_textures(SDL_Texture* player_texture, SDL_Texture* arm_texture);

	Object player_object;
	Object arm_object;
private:
	SDL_Rect left_clips[8];
	SDL_Rect right_clips[8];
	SDL_Rect left_arm_clip, right_arm_clip;

	uint32_t animation_frame_index = 0;
};
