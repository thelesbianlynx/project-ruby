#pragma once

#include <math.h>
#include <stdint.h>

/***
 ***    Vector-Math mini-Library.
 ***        - Definitions for 2d, 3d, and 4d vectors and operations.
 ***        - Definitions for float and int vectors.
 ***/

/*  Vector-2f Definition.
 */
typedef struct vec2f { float x; float y; } Vec2f;


/*  Vector-3f Definition.
 */
typedef struct vec3f { float x; float y; float z; } Vec3f;


/*  Vector-4f Definition.
 */
typedef struct vec4f { float x; float y; float z; float w; } Vec4f;


/*  Vector-2i Definition.
 */
typedef struct vec2i { int32_t i; int32_t j; } Vec2i;


/*  Vector-3i Definition.
 */
typedef struct vec3i { int32_t i; int32_t j; int32_t k; } Vec3i;


/*  Vector-4i Definition.
 */
typedef struct vec4i { int32_t i; int32_t j; int32_t k; int32_t l; } Vec4i;


/*  Vector Constructors.
 */
static inline
Vec2f cons2f (float x, float y) { return (Vec2f) {x, y}; }
static inline
Vec3f cons3f (float x, float y, float z) { return (Vec3f) {x, y, z}; }
static inline
Vec4f cons4f (float x, float y, float z, float w) { return (Vec4f) {x, y, z, w}; }

static inline
Vec2i cons2i (float i, float j) { return (Vec2i) {i, j}; }
static inline
Vec3i cons3i (float i, float j, float k) { return (Vec3i) {i, j, k}; }
static inline
Vec4i cons4i (float i, float j, float k, float l) { return (Vec4i) {i, j, k, l}; }

static inline
Vec2f cons2fi (Vec2i v) { return (Vec2f) {v.i, v.j}; }
static inline
Vec3f cons3fi (Vec3i v) { return (Vec3f) {v.i, v.j, v.k}; }
static inline
Vec4f cons4fi (Vec4i v) { return (Vec4f) {v.i, v.j, v.k, v.l}; }

static inline
Vec2i cons2if (Vec2f v) { return (Vec2i) {v.x, v.y}; }
static inline
Vec3i cons3if (Vec3f v) { return (Vec3i) {v.x, v.y, v.z}; }
static inline
Vec4i cons4if (Vec4f v) { return (Vec4i) {v.x, v.y, v.z, v.w}; }


/* Vector Addition.
 */
static inline
Vec2f add2f (Vec2f u, Vec2f v) { return (Vec2f) {u.x + v.x, u.y + v.y}; }
static inline
Vec3f add3f (Vec3f u, Vec3f v) { return (Vec3f) {u.x + v.x, u.y + v.y, u.z + v.z}; }
static inline
Vec4f add4f (Vec4f u, Vec4f v) { return (Vec4f) {u.x + v.x, u.y + v.y, u.z + v.z, u.w + v.w}; }

static inline
Vec2i add2i (Vec2i u, Vec2i v) { return (Vec2i) {u.i + v.i, u.j + v.j}; }
static inline
Vec3i add3i (Vec3i u, Vec3i v) { return (Vec3i) {u.i + v.i, u.j + v.j, u.k + v.k}; }
static inline
Vec4i add4i (Vec4i u, Vec4i v) { return (Vec4i) {u.i + v.i, u.j + v.j, u.k + v.k, u.l + v.l}; }


/* Vector Subtraction.
 */
static inline
Vec2f sub2f (Vec2f u, Vec2f v) { return (Vec2f) {u.x - v.x, u.y - v.y}; }
static inline
Vec3f sub3f (Vec3f u, Vec3f v) { return (Vec3f) {u.x - v.x, u.y - v.y, u.z - v.z}; }
static inline
Vec4f sub4f (Vec4f u, Vec4f v) { return (Vec4f) {u.x - v.x, u.y - v.y, u.z - v.z, u.w - v.w}; }

static inline
Vec2i sub2i (Vec2i u, Vec2i v) { return (Vec2i) {u.i - v.i, u.j - v.j}; }
static inline
Vec3i sub3i (Vec3i u, Vec3i v) { return (Vec3i) {u.i - v.i, u.j - v.j, u.k - v.k}; }
static inline
Vec4i sub4i (Vec4i u, Vec4i v) { return (Vec4i) {u.i - v.i, u.j - v.j, u.k - v.k, u.l - v.l}; }


/* Vector Multiplication.
 */



/* Vector Division.
 */



/* Vector Modulo.
 */



/* Vector Scale.
 */
static inline
Vec2f scale2f (float c, Vec2f v) { return (Vec2f) {c*v.x, c*v.y}; }
static inline
Vec3f scale3f (float c, Vec3f v) { return (Vec3f) {c*v.x, c*v.y, c*v.z}; }
static inline
Vec4f scale4f (float c, Vec4f v) { return (Vec4f) {c*v.x, c*v.y, c*v.z, c*v.w}; }

static inline
Vec2i scale2i (int32_t c, Vec2i v) { return (Vec2i) {c*v.i, c*v.j}; }
static inline
Vec3i scale3i (int32_t c, Vec3i v) { return (Vec3i) {c*v.i, c*v.j, c*v.k}; }
static inline
Vec4i scale4i (int32_t c, Vec4i v) { return (Vec4i) {c*v.i, c*v.j, c*v.k, c*v.l}; }


/* Vector Floor.
 */



/* Vector Ciel.
 */



/* Vector Round.
 */



/* Vector Dot-product.
 */
static inline
float dot2f (Vec2f u, Vec2f v) { return u.x * v.x + u.y * v.y; }
static inline
float dot3f (Vec3f u, Vec3f v) { return u.x * v.x + u.y * v.y + u.z * v.z; }
static inline
float dot4f (Vec4f u, Vec4f v) { return u.x * v.x + u.y * v.y + u.z * v.z + u.w * v.w; }


/* Vector Cross-product.
 */
static inline
Vec3f cross3f (Vec3f u, Vec3f v) {
    return (Vec3f) {
        u.y*v.z - u.z*v.y,
        u.z*v.x - u.x*v.z,
        u.x*v.y - u.y*v.x,
    };
}
static inline
Vec3f crossLeft3f (Vec3f u, Vec3f v) { return (Vec3f) { }; }


/* Vector Norm.
 */
static inline
float lengthSquared2f (Vec2f v) { return dot2f(v, v); }
static inline
float lengthSquared3f (Vec3f v) { return dot3f(v, v); }
static inline
float lengthSquared4f (Vec4f v) { return dot4f(v, v); }


/* Vector Length.
 */
static inline
float length2f (Vec2f v) { return sqrt(lengthSquared2f(v)); }
static inline
float length3f (Vec3f v) { return sqrt(lengthSquared3f(v)); }
static inline
float length4f (Vec4f v) { return sqrt(lengthSquared4f(v)); }


/* Vector Normalize.
 */
static inline
Vec2f normalize2f (Vec2f v) { return scale2f(1.0/length2f(v), v); }
static inline
Vec3f normalize3f (Vec3f v) { return scale3f(1.0/length3f(v), v); }
static inline
Vec4f normalize4f (Vec4f v) { return scale4f(1.0/length4f(v), v); }


/* Vector Angle Between Vectors.
 */
static inline
float angle2f (Vec2f u, Vec2f v) { return acos(dot2f(u, v) / (length2f(u)*length2f(v))); }
static inline
float angle3f (Vec3f u, Vec3f v) { return acos(dot3f(u, v) / (length3f(u)*length3f(v))); }
static inline
float angle4f (Vec4f u, Vec4f v) { return acos(dot4f(u, v) / (length4f(u)*length4f(v))); }
