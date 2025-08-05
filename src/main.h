#pragma once

//
// Standard Includes.
//
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "vector.h"
#include "matrix.h"

//
// Type Listing.
//
typedef struct array Array;

typedef struct window Window;
typedef struct shader Shader;
typedef struct shape Shape;
typedef struct image Image;
typedef struct drawinfo DrawInfo;
typedef struct vertexbuffer VertexBuffer;

typedef struct environment Environment;
typedef struct entity Entity;
typedef struct entity_type EntityType;
typedef struct message Message;

typedef struct player Player;

//
// Global Constants + Macros.
//
#define PI 3.1415926535897
#define TWO_PI 6.283185307179586
#define HALF_PI 1.57079632679

#define FOV 1.4
#define NEAR 0.03
#define FAR 300.0

#define SPEED 0.3
#define SENSITIVITY 0.003


static inline int32_t DIV (int32_t a, int32_t b) {
    return (int32_t) floor((float) a / (float) b);
}

static inline int32_t MOD (int32_t a, int32_t m) {
    return ((a % m) + m) % m;
}
