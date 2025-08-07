#include "player.h"

#include "entity.h"
#include "environment.h"
#include "render.h"

/*******************************************
Entity-Type Definition Template:

static void _init (Entity* entity);
static void _destroy (Entity* entity);
static void _load (Entity* entity);
static void _update (Entity* entity);
static void _save (Entity* entity);
static void _unload (Entity* entity);
static void _draw (Entity* entity, Shader* shader, DrawInfo* drawinfo);
static void _receive (Entity* entity, Entity* sender, Message* message);
static void _collide (Entity* entity, Entity* other);
static void _react (Entity* entity, Entity* other, float dist);

EntityType _entity_type = {
    .id = 0,
    .on_init = NULL,
    .on_destroy = NULL,
    .on_load = NULL,
    .on_update = NULL,
    .on_save = NULL,
    .on_unload = NULL,
    .on_draw = NULL,
    .on_receive = NULL,
    .on_collide = NULL,
    .on_react = NULL,
};
*******************************************/

static void player_init (Entity* entity);
static void player_destroy_ (Entity* entity);
static void player_load (Entity* entity);
static void player_update (Entity* entity);
static void player_save (Entity* entity);
static void player_unload (Entity* entity);
static void player_draw (Entity* entity, Shader* shader, DrawInfo* drawinfo);
static void player_receive (Entity* entity, Entity* sender, Message* message);
static void player_collide (Entity* entity, Entity* other);
static void player_react (Entity* entity, Entity* other, float dist);

EntityType player_entity_type = {
    .id = ENTITY_PLAYER,
    .on_init = player_init,
    .on_destroy = player_destroy_,
    .on_load = player_load,
    .on_update = player_update,
    .on_save = player_save,
    .on_unload = player_unload,
    .on_draw = player_draw,
    .on_receive = player_receive,
    .on_collide = player_collide,
    .on_react = player_react,
};

Player* player_create (Environment* env) {
    // Allocate and Initialize.
    Player* player = malloc(sizeof(Player));
    player->env = env;
    player->entity = entity_create(env, 0, ENTITY_PLAYER, cons3f(0,0,0));
    player->entity->data = player;

    player->yaw = 0;
    player->pitch = 0;

    return player;
}

static
void player_init (Entity* entity) {}

void player_destroy (Player* player) {
    entity_unref(player->entity);
}

static
void player_destroy_ (Entity* entity) {
    Player* player = entity->data;

    free(player);
}

static
void player_load (Entity* entity) {}

static
void player_update (Entity* entity) {
    Player* player = entity->data;
    Environment* env = entity->env;

    player->yaw -= SENSITIVITY * env->input->dmouse.x;
    player->pitch += SENSITIVITY * env->input->dmouse.y;

    while (player->yaw < 0) player->yaw += TWO_PI;
    while (player->yaw >= TWO_PI) player->yaw -= TWO_PI;

    if (player->pitch > HALF_PI) player->pitch = HALF_PI;
    if (player->pitch < -HALF_PI) player->pitch = -HALF_PI;

    float sy = SPEED * sin(player->yaw);
    float cy = SPEED * cos(player->yaw);

    if (env->input->up) {
        entity->pos.x -= sy;
        entity->pos.z -= cy;
    }
    if (env->input->down) {
        entity->pos.x += sy;
        entity->pos.z += cy;
    }
    if (env->input->left) {
        entity->pos.x -= cy;
        entity->pos.z += sy;
    }
    if (env->input->right) {
        entity->pos.x += cy;
        entity->pos.z -= sy;
    }

    if (env->input->space) {
        entity->pos.y += SPEED;
    }
    if (env->input->shift) {
        entity->pos.y -= SPEED;
    }


}

static
void player_save (Entity* entity) {}

static
void player_unload (Entity* entity) {}

Vec3f player_get_direction (Player* player) {
    float sy = sin(player->yaw);
    float cy = cos(player->yaw);

    float sp = sin(player->pitch);
    float cp = cos(player->pitch);

    return normalize3f(cons3f(cp*sy, sp, cp*cy));
}

void player_get_view (Player* player, Mat4f* V) {
    Vec3f t = player->entity->pos;
    Vec3f up = cons3f(0,1,0);

    Vec3f vz = player_get_direction(player);
    Vec3f vx = normalize3f(cross3f(up, vz));
    Vec3f vy = normalize3f(cross3f(vz, vx));

    *V = (Mat4f) {
        vx.x,  vx.y, vx.z, -dot3f(vx,t),
        vy.x,  vy.y, vy.z, -dot3f(vy,t),
        vz.x,  vz.y, vz.z, -dot3f(vz,t),
        0,     0,    0,     1,
    };

}

void player_draw_ui (Player* player) {
    // UI Goes Here...

}

static
void player_draw (Entity* entity, Shader* shader, DrawInfo* drawinfo) {}

static
void player_receive (Entity* entity, Entity* sender, Message* message) {}

static
void player_collide (Entity* entity, Entity* other) {}

static
void player_react (Entity* entity, Entity* other, float dist) {}
