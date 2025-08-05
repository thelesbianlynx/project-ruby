#include "main.h"

#include "window.h"


int main (int argc, char** argv) {
    window_init_backend();
    Window* window = window_create("Project Ruby", 1280, 720);

    // Game game;
    // game_init(&game, window);

    while (window_get_state(window)) {
        window_run_events(window);

        // game_update(&game);

        window_commit(window);
    }

    // game_end(&game);

    window_destroy(window);
    window_terminate_backend();
}
