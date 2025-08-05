#pragma once

#include "main.h"


struct environment {
    Window* window;
    Shader* shader;

    Player* player;

    uint32_t state;

    Array* entities;
    Array* new_entities;
};


Environment* env_create (Window* window);

void env_destroy (Environment* env);

void env_update (Environment* env);

void env_draw (Environment* env);

void env_add_entity (Environment* env, Entity* entity);
