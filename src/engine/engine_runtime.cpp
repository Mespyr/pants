#include "../include/engine.h"

bool Engine::running() { return !quit; }

void Engine::handle_events() {
    while (SDL_PollEvent(&e))
        if (e.type == SDL_QUIT) quit = true;
}

void Engine::update(float_t delta_time) {
    kbd_state = SDL_GetKeyboardState(nullptr);
    SDL_GetMouseState(&mouse_position.x, &mouse_position.y);

    if (kbd_state[SDL_SCANCODE_C]) map_reset();

    if (kbd_state[SDL_SCANCODE_W] || kbd_state[SDL_SCANCODE_S]) {
        if (kbd_state[SDL_SCANCODE_W])
            player.set_velocity_y(std::max<float>(
                player.get_velocity_y() - (PLAYER_ACCEL_SPEED * delta_time),
                -PLAYER_MAX_SPEED));
        if (kbd_state[SDL_SCANCODE_S])
            player.set_velocity_y(std::min<float>(
                player.get_velocity_y() + (PLAYER_ACCEL_SPEED * delta_time),
                PLAYER_MAX_SPEED));
    } else if (player.get_velocity_y() > 0)
        player.set_velocity_y(std::max<float>(
            player.get_velocity_y() - (PLAYER_ACCEL_SPEED * delta_time), 0.0f));
    else if (player.get_velocity_y() < 0)
        player.set_velocity_y(std::min<float>(
            player.get_velocity_y() + (PLAYER_ACCEL_SPEED * delta_time), 0.0f));

    if (kbd_state[SDL_SCANCODE_A] || kbd_state[SDL_SCANCODE_D]) {
        if (kbd_state[SDL_SCANCODE_A])
            player.set_velocity_x(std::max<float>(
                player.get_velocity_x() - (PLAYER_ACCEL_SPEED * delta_time),
                -PLAYER_MAX_SPEED));
        if (kbd_state[SDL_SCANCODE_D])
            player.set_velocity_x(std::min<float>(
                player.get_velocity_x() + (PLAYER_ACCEL_SPEED * delta_time),
                PLAYER_MAX_SPEED));
    } else if (player.get_velocity_x() > 0)
        player.set_velocity_x(std::max<float>(
            player.get_velocity_x() - (PLAYER_ACCEL_SPEED * delta_time), 0.0f));
    else if (player.get_velocity_x() < 0)
        player.set_velocity_x(std::min<float>(
            player.get_velocity_x() + (PLAYER_ACCEL_SPEED * delta_time), 0.0f));

    if (player_animation_timer >= 0.09) {
        player.update_animation_frame(mouse_position, window->center_x(),
                                      window->center_y());
        player_animation_timer = 0;
    } else
        player_animation_timer += delta_time;

    for (Object rect : map_objects) {
        if (!rect.collision || !is_visible(&rect)) continue;
        std::pair<ObjectCollisionSide, float_t> collision =
            check_collision(&player.object, &rect);
        switch (collision.first) {
        case ObjectCollisionSide::TOP_COLLISION:
            player.object.position.y -= collision.second;
            player.object.velocity.y = 0;
            break;
        case ObjectCollisionSide::BOTTOM_COLLISION:
            player.object.position.y += collision.second;
            player.object.velocity.y = 0;
            break;
        case ObjectCollisionSide::LEFT_COLLISION:
            player.object.position.x -= collision.second;
            player.object.velocity.x = 0;
            break;
        case ObjectCollisionSide::RIGHT_COLLISION:
            player.object.position.x += collision.second;
            player.object.velocity.x = 0;
            break;
        case ObjectCollisionSide::NO_COLLISION: break;
        }
    }

    player.object.update_position(delta_time);
}
