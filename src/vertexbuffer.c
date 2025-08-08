#include "vertexbuffer.h"

#include "render.h"

static void buffer_expand (VertexBuffer* buffer) {
    uint32_t new_capacity = buffer->capacity * 2;
    float* new_buffer = calloc(new_capacity, sizeof(void*));

    for (int i = 0; i < buffer->size; ++i)
        new_buffer[i] = buffer->data[i];

    free(buffer->data);
    buffer->data = new_buffer;
    buffer->capacity = new_capacity;
}

VertexBuffer* vertexbuffer_create () {
    VertexBuffer* buffer = malloc(sizeof(VertexBuffer));
    buffer->data = calloc(sizeof(float), 64);
    buffer->size = 0;
    buffer->capacity = 64;

    buffer->texcoord = cons2f(0,0);
    buffer->color = cons4f(0,0,0,0);
    buffer->normal = cons3f(0,0,0);

    return buffer;
}

void vertexbuffer_destroy (VertexBuffer* vertexbuffer) {
    free(vertexbuffer->data);
    free(vertexbuffer);
}

void vertexbuffer_texcoord (VertexBuffer* vertexbuffer, Vec2f texcoord) {
    vertexbuffer->texcoord = texcoord;
}

void vertexbuffer_color (VertexBuffer* vertexbuffer, Vec4f color) {
    vertexbuffer->color = color;
}

void vertexbuffer_normal (VertexBuffer* vertexbuffer, Vec3f normal) {
    vertexbuffer->normal = normal;
}

void vertexbuffer_vertex (VertexBuffer* vertexbuffer, Vec4f vertex) {
    while (vertexbuffer->size + 13 > vertexbuffer->capacity)
        buffer_expand(vertexbuffer);

    vertexbuffer->data[vertexbuffer->size]   = vertex.x;
    vertexbuffer->data[vertexbuffer->size+1] = vertex.y;
    vertexbuffer->data[vertexbuffer->size+2] = vertex.z;
    vertexbuffer->data[vertexbuffer->size+3] = vertex.w;
    vertexbuffer->data[vertexbuffer->size+4] = vertexbuffer->texcoord.x;
    vertexbuffer->data[vertexbuffer->size+5] = vertexbuffer->texcoord.y;
    vertexbuffer->data[vertexbuffer->size+6] = vertexbuffer->color.x;
    vertexbuffer->data[vertexbuffer->size+7] = vertexbuffer->color.y;
    vertexbuffer->data[vertexbuffer->size+8] = vertexbuffer->color.z;
    vertexbuffer->data[vertexbuffer->size+9] = vertexbuffer->color.w;
    vertexbuffer->data[vertexbuffer->size+10] = vertexbuffer->normal.x;
    vertexbuffer->data[vertexbuffer->size+11] = vertexbuffer->normal.y;
    vertexbuffer->data[vertexbuffer->size+12] = vertexbuffer->normal.z;

    vertexbuffer->size += 13;
}

void vertexbuffer_vertex3f (VertexBuffer* vertexbuffer, Vec3f vertex){
    vertexbuffer_vertex(vertexbuffer, cons4f(vertex.x, vertex.y, vertex.z, 1));
}

Shape* vertexbuffer_export (VertexBuffer* vertexbuffer, uint32_t type) {
    uint32_t size = vertexbuffer->size / 13;
    return shape_create(vertexbuffer->data, size, type);
}
