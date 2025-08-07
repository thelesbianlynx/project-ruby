#include "main.h"

#include "window.h"
#include "environment.h"

int main (int argc, char** argv) {
    window_init_backend();
    Window* window = window_create("Project Ruby", 1280, 720);
    window_grab_mouse(window, true);

    Environment* env = env_create(window);

    while (window_get_state(window)) {
        window_run_events(window);

        env_update(env);
        env_draw(env);

        window_commit(window);
    }

    env_destroy(env);

    window_destroy(window);
    window_terminate_backend();
}
