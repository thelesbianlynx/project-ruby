#pragma once

#include "main.h"


enum entity_list {
    ENTITY_PLAYER = 0,
};

enum entity_state {
    STATE_NORMAL,
    STATE_DESTROY,
};

enum entity_flags {
    // Set By Entity.
    FLAG_STATIC = 0x0001,

    // Set By Physics Engine.
    FLAG_GROUNDED = 0x0100,
};

struct entity {
    Environment* env;
    EntityType* type;

    // Entity ID, State, and RC.
    uint32_t id;
    uint32_t state;
    uint32_t refs;

    // Size (Spheroid)
    float radius;
    float height;

    // Motion State.
    Vec3f pos;
    Vec3f vel;
    uint32_t flags;

    // Motion on Ground.
    Vec3f motion;
    float friction;

    // Entity Awareness.
    float awareness;

    // Entity-Specific Data.
    void* data;
};


//
// Entity Event Functions.
//
typedef void (*entity_init_fn) (Entity* entity);
typedef void (*entity_destroy_fn) (Entity* entity);

typedef void (*entity_load_fn) (Entity* entity);
typedef void (*entity_update_fn) (Entity* entity);
typedef void (*entity_save_fn) (Entity* entity);
typedef void (*entity_unload_fn) (Entity* entity);

typedef void (*entity_draw_fn) (Entity* entity, Shader* shader, DrawInfo* drawinfo);

typedef void (*entity_receive_fn) (Entity* entity, Entity* sender, Message* Message);
typedef void (*entity_collide_fn) (Entity* entity, Entity* other);
typedef void (*entity_react_fn) (Entity* entity, Entity* other, float dist);

struct entity_type {
    // Entity Type-ID.
    uint32_t id;

    // Create/Destroy Events.
    entity_init_fn on_init;
    entity_destroy_fn on_destroy;

    // Basic Update Events.
    entity_load_fn on_load;
    entity_update_fn on_update;
    entity_save_fn on_save;
    entity_unload_fn on_unload;

    // Draw Event.
    entity_draw_fn on_draw;

    // Interaction with other Entities.
    entity_receive_fn on_receive;
    entity_collide_fn on_collide;
    entity_react_fn on_react;
};



Entity* entity_create (Environment* env, uint32_t id, uint32_t type_id, Vec3f pos);


void entity_ref (Entity* Entity);

void entity_unref (Entity* Entity);


void entity_load (Entity* entity);

void entity_update (Entity* entity);

void entity_save (Entity* entity);

void entity_unload (Entity* entity);


void entity_draw (Entity* entity, Shader* shader, DrawInfo* drawinfo);


void entity_send (Entity* entity, Entity* sender, Message* message);

void entity_collide (Entity* entity, Entity* other);

void entity_react (Entity* entity, Entity* other, float dist);
