#include "stdafx.h"
#include "Vec.h"

Vec3F h_normalize(const Vec4F& other) {
    Vec3F result;
    float norm = 0.0f;
    for (int i = 0; i < 3; i++) // don't include w
        norm += other[i] * other[i];

    norm = std::sqrt(norm);
    result[0] = other[0];
    result[1] = other[1];
    result[2] = other[2];
    result /= norm;
    
    return result;
}

Vec4F h_cross(const Vec4F& a, const Vec4F& b) {
    Vec4F result;
    result[0] = a[1] * b[2] - a[2] * b[1];
    result[1] = a[2] * b[0] - a[0] * b[2];
    result[2] = a[0] * b[1] - a[1] * b[0];
    result[3] = a[3] * b[3];

    return result;
}

Vec4F h_sub(const Vec4F& a, const Vec4F& b) {
    Vec4F result;
    result[0] = a[0] * b[3] - b[0] * a[3];
    result[1] = a[1] * b[3] - b[1] * a[3];
    result[2] = a[2] * b[3] - b[2] * a[3];
    result[3] = a[3] * b[3];

    return result;
}

Vec3F h_flatten(const Vec4F& a) {
    return {
        a[0] / a[3],
        a[1] / a[3],
        a[2] / a[3]
    };
}
