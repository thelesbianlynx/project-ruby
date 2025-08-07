#pragma once

#include "main.h"


struct environment {
    Window* window;
    Shader* shader;

    InputState* input;

    Player* player;

    uint32_t state;

    Array* entities;
    Array* new_entities;
};

struct input_state {
    Vec2f mouse;
    Vec2f dmouse;

    bool up;
    bool down;
    bool left;
    bool right;

    bool space;
    bool shift;
};

Environment* env_create (Window* window);

void env_destroy (Environment* env);

void env_update (Environment* env);

void env_draw (Environment* env);

void env_add_entity (Environment* env, Entity* entity);
