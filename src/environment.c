#include "environment.h"

#include "array.h"
#include "window.h"
#include "render.h"
#include "entity.h"
#include "player.h"


static void on_key (Window* window, uint32_t key, uint32_t state);
static void on_mouse_hover (Window* window, bool hover);
static void on_mouse_motion (Window* window, double x, double y);
static void on_mouse_button (Window* window, uint32_t button, uint32_t state);
static void on_mouse_scroll (Window* window, double sx, double sy);
static void on_focus_change (Window* window, bool focus);
static void on_resize (Window* window, int width, int height);

static void get_projection (int width, int height, Mat4f* P);

enum {
    ENV_INIT,
    ENV_PRELOAD,
    ENV_LOAD,
    ENV_RUN,
    ENV_UNLOAD,
};

Environment* env_create (Window* window) {
    // Allocate and Initialize.
    Environment* env = malloc(sizeof(Environment));
    env->window = window;
    env->shader = shader_create("res/shader/default");
    env->player = player_create(env);
    env->entities = array_create();
    env->new_entities = array_create();

    window->events.on_key_event = on_key;
    window->events.on_mouse_hover_event = on_mouse_hover;
    window->events.on_mouse_motion_event = on_mouse_motion;
    window->events.on_mouse_button_event = on_mouse_button;
    window->events.on_mouse_scroll_event = on_mouse_scroll;
    window->events.on_focus_change_event = on_focus_change;
    window->events.on_resize_event = on_resize;
    window->user = env;

    env->state = ENV_INIT;

    int width, height;
    window_get_size(window, &width, &height);

    Mat4f P;
    get_projection(width, height, &P);

    shader_set_projection(env->shader, &P);

    return env;
}

void env_destroy (Environment* env) {
    for (int i = 0; i < env->entities->size; ++i) {
        entity_unload(env->entities->data[i]);
        entity_unref(env->entities->data[i]);
    }

    for (int i = 0; i < env->new_entities->size; ++i) {
        entity_unref(env->new_entities->data[i]);
    }

    array_destroy(env->entities);
    array_destroy(env->new_entities);
    player_destroy(env->player);
    shader_destroy(env->shader);
    free(env);
}

void env_update (Environment* env) {
    if (env->state == ENV_INIT) {

        env->state = ENV_PRELOAD;
    }

    if (env->state == ENV_PRELOAD) {

        env->state = ENV_LOAD;
    }

    if (env->state == ENV_LOAD) {
        
        entity_ref(env->player->entity);
        array_add(env->entities, env->player->entity);

        for (int i = 0; i < env->entities->size; ++i) {
            entity_load(env->entities->data[i]);
        }

        env->state = ENV_RUN;
    }

    if (env->state == ENV_RUN) {
        // Entity Update.
        for (int i = 0; i < env->entities->size; ++i) {
            entity_update(env->entities->data[i]);
        }

        // Entity Awareness.
        // ...

        // Entity Physics.
        // ...

        // Add New Entities.
        for (int i = 0; i < env->new_entities->size; ++i) {
            entity_load(env->new_entities->data[i]);
            array_add(env->entities, env->new_entities->data[i]);
        }
        array_clear(env->new_entities);

        // Remove Stale Entities.
        for (int i = 0; i < env->entities->size;) {
            Entity* e = env->entities->data[i];
            if (e->state == STATE_DESTROY) {
                array_remove(env->entities, i);
                entity_unload(e);
                entity_unref(e);
            } else {
                i++;
            }
        }

    }

    if (env->state == ENV_UNLOAD) {
        for (int i = 0; i < env->entities->size; ++i) {
            entity_save(env->entities->data[i]);
            entity_unload(env->entities->data[i]);
            entity_unref(env->entities->data[i]);
        }

        for (int i = 0; i < env->new_entities->size; ++i) {
            entity_unref(env->new_entities->data[i]);
        }

        array_clear(env->entities);
        array_clear(env->new_entities);

        env->state = ENV_INIT;
    }
}

void env_draw (Environment* env) {
    if (env->state == ENV_RUN) {
        Mat4f V;
        player_get_view(env->player, &V);
        shader_set_view(env->shader, &V);

        DrawInfo info;
        drawinfo_init(&info);
        for (int i = 0; i < env->entities->size; i++) {
            drawinfo_init(&info);
            entity_draw(env->entities->data[i], env->shader, &info);
        }
    }

    player_draw_ui(env->player);
}

void env_add_entity (Environment* env, Entity* entity) {
    entity_ref(entity);
    array_add(env->new_entities, entity);
}


static
void on_key (Window* window, uint32_t key, uint32_t state) {

}

static
void on_mouse_hover (Window* window, bool hover) {

}

static
void on_mouse_motion (Window* window, double x, double y) {

}

static
void on_mouse_button (Window* window, uint32_t button, uint32_t state) {

}

static
void on_mouse_scroll (Window* window, double sx, double sy) {

}

static
void on_focus_change (Window* window, bool focus) {

}

static
void on_resize (Window* window, int width, int height) {
    Environment* env = window->user;

    Mat4f P;
    get_projection(width, height, &P);
    shader_set_projection(env->shader, &P);
}

static
void get_projection (int width, int height, Mat4f* P) {
    float a = (float) width / (float) height;
    float f = 1.0/(float)tan(FOV/2);
    float d = FAR - NEAR;

    *P = (Mat4f) { f/a,      0,  0,             0,
                   0,        f,  0,             0,
                   0,        0, -(FAR+NEAR)/d, -2*FAR*NEAR/d,
                   0,        0, -1,             0
    };
}
