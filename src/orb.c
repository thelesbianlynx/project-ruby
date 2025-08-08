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

    // {
    //     VertexBuffer* buf = vertexbuffer_create();
    //
    //     vertexbuffer_color(buf, cons4f(1,1,1,1));
    //     vertexbuffer_vertex(buf, cons4f(0,0,0,1));
    //     vertexbuffer_vertex(buf, cons4f(1,1,0,1));
    //     vertexbuffer_vertex(buf, cons4f(0,1,0,1));
    //     vertexbuffer_vertex(buf, cons4f(0,0,0,1));
    //     vertexbuffer_vertex(buf, cons4f(1,0,0,1));
    //     vertexbuffer_vertex(buf, cons4f(1,1,0,1));
    //
    //     orb->shape = vertexbuffer_export(buf, GL_TRIANGLES);
    //     vertexbuffer_destroy(buf);
    // }
    orb->shape = mkOrbShape();
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
    VertexBuffer* buf = vertexbuffer_create();

    vertexbuffer_color(buf, cons4f(1,1,1,1));

    #define STEPS 50
    #define RINGS 12

    for (int i = 1; i < RINGS + 1; i++) {

        float p = i * PI / (RINGS + 1);
        p -= HALF_PI;

        for (int j = 0; j < STEPS; j++) {
            float y1 = j * TWO_PI/STEPS;
            float y2 = (j+1) * TWO_PI/STEPS;

            float p1 = p + (HALF_PI/96);
            float p2 = p - (HALF_PI/96);

            Vec3f v11 = dirvec(p1, y1);
            Vec3f v12 = dirvec(p1, y2);
            Vec3f v21 = dirvec(p2, y1);
            Vec3f v22 = dirvec(p2, y2);

            vertexbuffer_normal(buf, v11);
            vertexbuffer_vertex3f(buf, v11);
            vertexbuffer_normal(buf, v21);
            vertexbuffer_vertex3f(buf, v21);
            vertexbuffer_normal(buf, v12);
            vertexbuffer_vertex3f(buf, v12);
            vertexbuffer_normal(buf, v21);
            vertexbuffer_vertex3f(buf, v21);
            vertexbuffer_normal(buf, v22);
            vertexbuffer_vertex3f(buf, v22);
            vertexbuffer_normal(buf, v12);
            vertexbuffer_vertex3f(buf, v12);
        }
    }


    Shape* orb = vertexbuffer_export(buf, GL_TRIANGLES);
    vertexbuffer_destroy(buf);

    return orb;
}
