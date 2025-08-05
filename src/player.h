#pragma once

#include "main.h"


struct player {
    Environment* env;
    Entity* entity;


    float yaw, pitch;
};


Player* player_create (Environment* env);

void player_destroy (Player* player);


Vec3f player_get_direction (Player* player);

void player_get_view (Player* player, Mat4f* V);


void player_draw_ui (Player* player);
