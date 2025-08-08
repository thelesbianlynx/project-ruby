#pragma once

#include "main.h"


struct vertexbuffer {
    float* data;

    uint32_t size;
    uint32_t capacity;

    Vec2f texcoord;
    Vec4f color;
    Vec3f normal;
};

VertexBuffer* vertexbuffer_create ();
void vertexbuffer_destroy (VertexBuffer* vertexbuffer);

void vertexbuffer_texcoord (VertexBuffer* vertexbuffer, Vec2f texcoord);
void vertexbuffer_color (VertexBuffer* vertexbuffer, Vec4f color);
void vertexbuffer_normal (VertexBuffer* vertexbuffer, Vec3f normal);

void vertexbuffer_vertex (VertexBuffer* vertexbuffer, Vec4f vertex);
void vertexbuffer_vertex3f (VertexBuffer* vertexbuffer, Vec3f vertex);

Shape* vertexbuffer_export (VertexBuffer*, uint32_t type);
