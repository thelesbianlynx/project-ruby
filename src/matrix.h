#pragma once

/*  Matrix-2x2 Definition.
 */
typedef struct mat2f {
    float ax; float bx;
    float ay; float by;
} Mat2f;


/*  Matrix-3x3f Definition.
 */
typedef struct mat3f {
    float ax; float bx; float cx;
    float ay; float by; float cy;
    float az; float bz; float cz;
} Mat3f;


/*  Matrix-4x4f Definition.
 */
typedef struct mat4f {
    float ax; float bx; float cx; float dx;
    float ay; float by; float cy; float dy;
    float az; float bz; float cz; float dz;
    float aw; float bw; float cw; float dw;
} Mat4f;
