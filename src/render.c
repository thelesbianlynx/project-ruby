#include "render.h"

#include "lodepng.h"

// Static helper to load and compile a shader unit from a file.
static GLuint load_shader_file (const char* file, GLenum stage) {
    // Open file.
    FILE* f = fopen(file, "r");
    if (f == NULL) {
        printf("Cannot open shader file: %s\n", file);
        return 0;
    }

    // Create Buffer (4k max size).
    int max_size = 1024*4;
    char text[max_size+1];

    // Read from file.
    int c;
    int text_size = 0;
    while (text_size < max_size && (c = fgetc(f)) != EOF) {
        text[text_size] = c;
        text_size++;
    }
    text[text_size] = '\0';

    // Close File.
    fclose(f);

    // Create Shader.
    GLuint shader = glCreateShader(stage);

    // Upload Shader Text.
    const char* text_list [] = { text };
    glShaderSource(shader, 1, text_list , &text_size);

    // Compile Shader.
    glCompileShader(shader);

    // Error-Check.
    int result;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        printf("SHADER COMPILE ERROR: %s\n", file);
        glGetShaderInfoLog(shader, max_size, NULL, text);
        printf("%s\n", text);
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

Shader* shader_create (const char* name) {
    // Shader file name buffer.
    int len = strlen(name) + 24;
    char file[len];

    // Vertex Shader.
    snprintf(file, len, "%s.vs", name);
    GLuint vs = load_shader_file(file, GL_VERTEX_SHADER);
    if (vs == 0) {
        return NULL;
    }

    // Fragment Shader.
    snprintf(file, len, "%s.fs", name);
    GLuint fs = load_shader_file(file, GL_FRAGMENT_SHADER);
    if (fs == 0) {
        glDeleteShader(vs);
        return NULL;
    }

    // Shader Program.
    GLuint shader_id = glCreateProgram();

    // Attach and Link.
    glAttachShader(shader_id, vs);
    glAttachShader(shader_id, fs);
    glLinkProgram(shader_id);

    int result;
    glGetProgramiv(shader_id, GL_LINK_STATUS, &result);
    if (result == GL_FALSE) {
        char text[4096];
        printf("SHADER LINK ERROR\n");
        glGetProgramInfoLog(shader_id, 4096, NULL, text);
        printf("%s\n", text);
        glDeleteShader(shader_id);
        return NULL;
    }

    // Delete Shaders.
    glDeleteShader(vs);
    glDeleteShader(fs);

    // Allocate and Initialize.
    Shader* shader = malloc(sizeof(struct shader));
    shader->shader_id = shader_id;

    // Uniform Locations.
    shader->uniforms.projection = glGetUniformLocation(shader_id, "P");
    shader->uniforms.view = glGetUniformLocation(shader_id, "V");
    shader->uniforms.model = glGetUniformLocation(shader_id, "M");
    shader->uniforms.obj_color = glGetUniformLocation(shader_id, "C");
    shader->uniforms.obj_texture = glGetUniformLocation(shader_id, "T");
    shader->uniforms.image = glGetUniformLocation(shader_id, "image");
    shader->uniforms.use_image = glGetUniformLocation(shader_id, "use_image");

    // Slot Initialization.
    for (int i = 0; i < SHADER_MAX_SHAPES; i++) {
        shader->shapes[i] = NULL;
    }
    for (int i = 0; i < SHADER_MAX_IMAGES; i++) {
        shader->images[i] = NULL;
    }

    // Image Preparation.
    glUseProgram(shader_id);
    glUniform1i(shader->uniforms.image, 0);
    glUseProgram(0);

    return shader;
}

void shader_destroy (Shader* shader) {
    for (int i = 0; i < SHADER_MAX_SHAPES; i++) {
        if (shader->shapes[i] != NULL)
            shape_destroy(shader->shapes[i]);
    }
    for (int i = 0; i < SHADER_MAX_IMAGES; i++) {
        if (shader->images[i] != NULL)
            image_destroy(shader->images[i]);
    }
    glDeleteProgram (shader->shader_id);
    free(shader);
}

Shape* shape_create (const float* data, GLuint size, GLenum type) {
    // VAO and VBO IDs.
    GLuint vao_id, vbo_id;

    // Create VAO.
    glGenVertexArrays(1, &vao_id);
    glBindVertexArray(vao_id);

    // Create VBO.
    glGenBuffers(1, &vbo_id);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);

    // Upload Buffer Data.
    GLuint length = size * 13 * sizeof(float);
    glBufferData(GL_ARRAY_BUFFER, length, data, GL_STATIC_DRAW);

    // Bind Attributes.
    glEnableVertexAttribArray(ATTRIB_POSITION);
    glVertexAttribPointer(ATTRIB_POSITION, 4, GL_FLOAT, GL_FALSE, 52, 0);
    glEnableVertexAttribArray(ATTRIB_TEXCOORD);
    glVertexAttribPointer(ATTRIB_TEXCOORD, 2, GL_FLOAT, GL_FALSE, 52, (void*)16);
    glEnableVertexAttribArray(ATTRIB_COLOR);
    glVertexAttribPointer(ATTRIB_COLOR, 4, GL_FLOAT, GL_FALSE, 52, (void*)24);
    glEnableVertexAttribArray(ATTRIB_NORMAL);
    glVertexAttribPointer(ATTRIB_NORMAL, 3, GL_FLOAT, GL_FALSE, 52, (void*)40);

    // Cleanup.
    glBindVertexArray(0);

    // Allocate and Initalize.
    Shape* shape = malloc(sizeof(struct shape));
    shape->vao_id = vao_id;
    shape->vbo_id = vbo_id;
    shape->size = size;
    shape->type = type;

    return shape;
}

void shape_destroy (Shape* shape) {
    glDeleteBuffers(1, &shape->vbo_id);
    glDeleteVertexArrays(1, &shape->vao_id);
    free(shape);
}

Image* image_create (const char* file) {
    // Decoded Image Data.
    uint8_t* data;

    // Size of image.
    GLuint width, height;

    // Decode.
    uint32_t result = lodepng_decode32_file(&data, &width, &height, file);
    if (result != 0) {
        printf("Texture Load Error %d\n", result);
        return NULL;
    }

    // Generate OpenGL Texture to store the image.
    GLuint image_id;
    glGenTextures(1, &image_id);

    // Bind Texture.
    glBindTexture(GL_TEXTURE_2D, image_id);

    // Upload Image Data.
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    // Set Texture Parameters.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Unbind Texture.
    glBindTexture(GL_TEXTURE_2D, 0);

    // Release Image Data.
    free(data);

    // Allocate and Initialize.
    Image* image = malloc(sizeof(Image));
    image->image_id = image_id;
    image->width = width;
    image->height = height;

    // Return
    return image;
}

void image_destroy (Image* image) {
    glDeleteTextures(1, &image->image_id);
    free(image);
}

void drawinfo_init (DrawInfo* drawinfo) {
    *drawinfo = (DrawInfo) {
        .shape = NULL,
        .model = (Mat4f) {
            1,0,0,0,
            0,1,0,0,
            0,0,1,0,
            0,0,0,1,
        },
        .image = NULL,
        .color = (Vec4f) {1,1,1,1},
        .texture = (Vec4f) {0,0,1,1},
        .enable_depthtest = true,
        .enable_depthmask = false,
        .enable_culling = true,
    };
}

void shader_set_projection (Shader* shader, Mat4f* p) {
    glUseProgram(shader->shader_id);
    // float data[] = {
    //     p->ax, p->bx, p->cx, p->dx,
    //     p->ay, p->by, p->cy, p->dy,
    //     p->az, p->bz, p->cz, p->dz,
    //     p->aw, p->bw, p->cw, p->dw
    // };
    glUniformMatrix4fv(shader->uniforms.projection, 1, GL_TRUE, (float*) p);
    glUseProgram(0);
}

void shader_set_view (Shader* shader, Mat4f* v) {
    glUseProgram(shader->shader_id);
    // float data[] = {
    //     v->ax, v->bx, v->cx, v->dx,
    //     v->ay, v->by, v->cy, v->dy,
    //     v->az, v->bz, v->cz, v->dz,
    //     v->aw, v->bw, v->cw, v->dw
    // };
    glUniformMatrix4fv(shader->uniforms.view, 1, GL_TRUE, (float*) v);
    glUseProgram(0);
}

void shader_draw (Shader* shader, struct drawinfo* drawinfo) {
    // Bind Shader.
    glUseProgram(shader->shader_id);

    // Model Matrix.
    // Mat4f* m = &drawinfo->obj_transform;
    // float data[] = {
    //     m->ax, m->bx, m->cx, m->dx,
    //     m->ay, m->by, m->cy, m->dy,
    //     m->az, m->bz, m->cz, m->dz,
    //     m->aw, m->bw, m->cw, m->dw
    // };
    glUniformMatrix4fv(shader->uniforms.model, 1, GL_TRUE, (float*) &drawinfo->model);

    // Color Transform.
    Vec4f* c = &drawinfo->color;
    glUniform4f(shader->uniforms.obj_color, c->x, c->y, c->z, c->w);

    // Texture Transform
    Vec4f* t = &drawinfo->texture;
    glUniform4f(shader->uniforms.obj_texture, t->x, t->y, t->z, t->w);

    // Depth Flag.
    if (drawinfo->enable_depthtest) {
        glEnable(GL_DEPTH_TEST);
    } else {
        glDisable(GL_DEPTH_TEST);
    }

    // Culling
    if (drawinfo->enable_culling) {
        glEnable(GL_CULL_FACE);
    } else {
        glDisable(GL_CULL_FACE);
    }

    // Image.
    if (drawinfo->image != NULL) {
        glBindTexture(GL_TEXTURE_2D, drawinfo->image->image_id);
        glUniform1i(shader->uniforms.use_image, 1);
    } else {
        glUniform1i(shader->uniforms.use_image, 0);
    }

    // Shape to draw.
    Shape* shape = drawinfo->shape;

    // Bind Array.
    glBindVertexArray(shape->vao_id);

    // Draw.
    glDrawArrays(shape->type, 0, shape->size);

    // Unbind Array.
    glBindVertexArray(0);

    // Unbind Texture.
    glBindTexture(GL_TEXTURE_2D, 0);

    // Unbind Shader.
    glUseProgram(0);
}
