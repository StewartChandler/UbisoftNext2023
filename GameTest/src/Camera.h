#ifndef _CAMERA_H
#define _CAMERA_H
#include "Mat.h"
#include "Mesh.h"

class Camera
{
private:
    /* data */
    Mat4F view_mat;
    Mat4F proj_mat;
public:
    Camera(const Vec3F& eye, const Vec3F& target);
    void lookat(const Vec3F& eye, const Vec3F& target);
    void render(
        const Mesh& m,
        const Vec3F& C1,
        const Vec3F& C2,
        const Mat4F& model_mat
    );
};

#endif