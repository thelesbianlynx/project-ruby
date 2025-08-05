#include "entity.h"

#include "environment.h"


extern EntityType player_entity_type;
extern EntityType orb_entity_type;

EntityType* entity_type_list[] = {
    [ENTITY_PLAYER] = &player_entity_type,
    [ENTITY_ORB] = &orb_entity_type,
};

Entity* entity_create (Environment* env, uint32_t id, uint32_t type_id, Vec3f pos) {
    // Allocate and Initialize.
    Entity* entity = malloc(sizeof(Entity));
    entity->env = env;
    entity->type = entity_type_list[type_id];
    entity->id = id;
    entity->refs = 1;
    entity->state = STATE_NORMAL;
    entity->radius = 0;
    entity->height = 0;
    entity->pos = pos;
    entity->vel = cons3f(0,0,0);
    entity->flags = 0;
    entity->motion = cons3f(0,0,0);
    entity->friction = 0;
    entity->awareness = 0;
    entity->data = NULL;

    // Type-Specific Initializer.
    if (entity->type->on_init != NULL) {
        entity->type->on_init(entity);
    }

    // Return.
    return entity;
}

void entity_ref (Entity* entity) {
    // Increment Reference Count.
    entity->refs++;
}

void entity_unref (Entity* entity) {
    if (entity == NULL) return;

    // Decrement Reference Count.
    entity->refs--;

    // If Reference Count is 0, Destroy Entity.
    if (entity->refs == 0) {
        // Notify Entity it's about to be destroyed.
        if (entity->type->on_destroy != NULL) {
            entity->type->on_destroy(entity);
        }

        // Free.
        free(entity);
    }
}

void entity_load (Entity* entity) {
    if (entity->type->on_load != NULL) {
        entity->type->on_load(entity);
    }
}

void entity_update (Entity* entity) {
    if (entity->type->on_update != NULL) {
        entity->type->on_update(entity);
    }
}

void entity_save (Entity* entity) {
    if (entity->type->on_save != NULL) {
        entity->type->on_save(entity);
    }
}

void entity_unload (Entity* entity) {
    if (entity->type->on_unload != NULL) {
        entity->type->on_unload(entity);
    }
}

void entity_draw (Entity* entity, Shader* shader, DrawInfo* drawinfo) {
    if (entity->type->on_draw != NULL) {
        entity->type->on_draw(entity, shader, drawinfo);
    }
}

void entity_send (Entity* entity, Entity* sender, Message* message) {
    if (entity->type->on_receive != NULL) {
        entity->type->on_receive(entity, sender, message);
    }
}

void entity_collide (Entity* entity, Entity* other) {
    if (entity->type->on_collide != NULL) {
        entity->type->on_collide(entity, other);
    }
}

void entity_react (Entity* entity, Entity* other, float dist) {
    if (entity->type->on_react != NULL) {
        entity->type->on_react(entity, other, dist);
    }
}
