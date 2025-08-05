#pragma once

#include "main.h"

#include <glad/glad.h>

// SHAPE BUFFER FORMAT
//
// - A shape is defined by a list of vertices, each of which is 13 floats long.
// - The floats of a vertex have the following meaning:
//
//    (x,y,z,w,u,v,r,g,b,a,nx,ny,nz)
//
//      where:
//       - (x,y,z,w) is the position.
//       - (u,v) is the texture coordinate.
//       - (r,g,b,a) is the color.
//       - (nx, ny, nz) is the normal.
//
// - Thus, every buffer passed into shape_create must have a length that is a multiple of 13.
// - Further, the meaning of parameter 'size' is the number of vertices, not the length of the list directly.


// Standard Uniforms.
struct shader_uniforms {
    GLint projection;           // Type: mat4
    GLint view;                 // Type: mat4
    GLint model;                // Type: mat4
    GLint obj_color;            // Type: vec4
    GLint obj_texture;          // Type: vec4
    GLint image;                // Type: sampler2d
    GLint use_image;            // Type: int
};

// Attribute Locations.
enum shader_attributes {
    ATTRIB_POSITION = 1,        // Type: vec4
    ATTRIB_TEXCOORD = 2,        // Type: vec2
    ATTRIB_COLOR = 3,           // Type: vec4
    ATTRIB_NORMAL = 4,          // Type: vec3
};

// Resource Slots.
enum {
    SHADER_MAX_SHAPES = 16,
    SHADER_MAX_IMAGES = 128,
};

// Shader Object.
//  - Holds shader id and attribute + uniform locations.
//  - Has Slots for storing frequently used resources (shapes and images).
struct shader {
    GLuint shader_id;

    struct shader_uniforms uniforms;

    Shape* shapes[SHADER_MAX_SHAPES];
    Image* images[SHADER_MAX_IMAGES];
};

// Shape Object.
//  - Stores VAO and VBO handle + size and type of the data.
struct shape {
    GLuint vao_id;
    GLuint vbo_id;

    GLuint size;    // Number of vertices

    GLenum type;    // One of GL_TRIANGLES, GL_LINES or GL_POINTS.
};

// Image Object.
struct image {
    // Image ID.
    GLuint image_id;

    // Image Size.
    GLuint width;
    GLuint height;
};

// Drawing Info.
//  - Stores all the parameters for a single draw call.
struct drawinfo {
    // Shape Object.
    Shape* shape;

    // Image Object.
    Image* image;

    // Transforms.
    Mat4f model;
    Vec4f color;
    Vec4f texture;

    // Depth Test Flags.
    bool enable_depthtest;
    bool enable_depthmask;

    // Backface Culling.
    bool enable_culling;
};

// Create and Destroy Shader Objects.
Shader* shader_create (const char* name);
void shader_destroy (Shader* shader);

// Create and Destroy Shape Objets
//  - Length of list 'data' must be size*13 floats long.
Shape* shape_create (const float* data, GLuint size, GLenum type);
void shape_destroy (Shape* shape);


// Create and Destory Image Objects.
Image* image_create (const char* file);
void image_destroy (Image* image);

// Initialize DrawInfo with default values.
void drawinfo_init (DrawInfo* drawinfo);

// Render functions.
void shader_set_projection (Shader* shader, Mat4f* projection);
void shader_set_view (Shader* shader, Mat4f* view);
void shader_draw (Shader* shader, DrawInfo* drawinfo);
