#include "stdafx.h"
#include "Mat.h"

// source: 
// https://learn.microsoft.com/en-us/windows/win32/opengl/glfrustum
Mat4F AffineProj(
    const float near,
    const float far,
    const float aspect
) {
    const float r = 1.0;
    const float t = aspect;
    const float a = (far + near) / (near - far);
    const float b = (2 * far * near) / (near - far);

    return Mat4F {
        near / r, 0.0f,     0.0f,     0.0f,
        0.0f,     near / t, 0.0f,     0.0f,
        0.0f,     0.0f,     a,        b,
        0.0f,     0.0f,     -1.0f,    0.0f
    };
}

Mat4F AffineTranslation(const Vec3F& trans) {
    return {
        1.0f, 0.0f, 0.0f, trans[0],
        0.0f, 1.0f, 0.0f, trans[1],
        0.0f, 0.0f, 1.0f, trans[2],
        0.0f, 0.0f, 0.0f, 1.0f
    };
}

Mat4F AffineRotation(const float phi) {
    return {
        cos(phi), 0.0f, -sin(phi), 0.0f,
        0.0f,     1.0f, 0.0f,      0.0f,
        sin(phi), 0.0f, cos(phi),  0.0f,
        0.0f,     0.0f, 0.0f,      1.0f
    };
}
