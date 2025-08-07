#include "orb.h"

#include "entity.h"
#include "environment.h"
#include "render.h"
#include "vertexbuffer.h"


static void orb_init (Entity* entity);
static void orb_destroy (Entity* entity);
// static void orb_load (Entity* entity);
// static void orb_update (Entity* entity);
// static void orb_save (Entity* entity);
// static void orb_unload (Entity* entity);
static void orb_draw (Entity* entity, Shader* shader, DrawInfo* drawinfo);
// static void orb_receive (Entity* entity, Entity* sender, Message* message);
// static void orb_collide (Entity* entity, Entity* other);
// static void orb_react (Entity* entity, Entity* other, float dist);

static Shape* mkOrbShape ();

EntityType orb_entity_type = {
    .id = 0,
    .on_init = orb_init,
    .on_destroy = orb_destroy,
    .on_load = NULL,
    .on_update = NULL,
    .on_save = NULL,
    .on_unload = NULL,
    .on_draw = orb_draw,
    .on_receive = NULL,
    .on_collide = NULL,
    .on_react = NULL,
};


static
void orb_init (Entity* entity) {
    Orb* orb = malloc(sizeof(Orb));

    {
        VertexBuffer* buf = vertexbuffer_create();

        vertexbuffer_color(buf, cons4f(1,1,1,1));
        vertexbuffer_vertex(buf, cons4f(0,0,0,1));
        vertexbuffer_vertex(buf, cons4f(1,1,0,1));
        vertexbuffer_vertex(buf, cons4f(0,1,0,1));
        vertexbuffer_vertex(buf, cons4f(0,0,0,1));
        vertexbuffer_vertex(buf, cons4f(1,0,0,1));
        vertexbuffer_vertex(buf, cons4f(1,1,0,1));

        orb->shape = vertexbuffer_export(buf, GL_TRIANGLES);
        vertexbuffer_destroy(buf);
    }
    entity->data = orb;
}


static
void orb_destroy (Entity* entity) {
    Orb* orb = entity->data;
    shape_destroy(orb->shape);
    free(orb);
}

static
void orb_draw (Entity* entity,  Shader* shader, DrawInfo* drawinfo) {
    Orb* orb = entity->data;

    drawinfo->shape = orb->shape;
    drawinfo->color = cons4f(0,1,1,1);
    drawinfo->enable_culling = false;
    drawinfo->model = (Mat4f) {
        1, 0, 0, entity->pos.x,
        0, 1, 0, entity->pos.y,
        0, 0, 1, entity->pos.z,
        0, 0, 0, 1,
    };
    shader_draw(shader, drawinfo);
}


//
// Orb Shape Code.
//

static
Vec3f dirvec (float pitch, float yaw) {
    float sy = sin(yaw);
    float cy = cos(yaw);

    float sp = sin(pitch);
    float cp = cos(pitch);

    return normalize3f(cons3f(cp*sy, sp, cp*cy));
}

static
Shape* mkOrbShape () {
    // VertexBuffer* buf = vertexbuffer_create();
    //
    // #define STEPS 50
    //
    // for (int i = 0; i < STEPS) {
    //
    // }
    //
    // Shape* orb = vertexbuffer_export(buf, GL_TRIANGLES);
    // vertexbuffer_destroy(buf);
    //
    // return orb;
}
