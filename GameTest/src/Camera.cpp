#include "stdafx.h"
#include "Camera.h"
#include "..\app\app.h"
#include "..\app\AppSettings.h"

#define MY_NEAR 1.0f
#define MY_FAR 4.0f

static Vec3F dir_vec(float x, float y) {
    const float d = (MY_FAR / MY_NEAR - 1);
    Vec3F result{x * d, y * d, MY_FAR - MY_NEAR};
    result.normalize();
    return result;
}

void Camera::lookat(const Vec3F& eye, const Vec3F& target) {
    Vec3F zaxis = target - eye;
    zaxis.normalize();
    Vec3F xaxis = Vec3F{0.0f, 1.0f, 0.0f}.cross(zaxis);
    xaxis.normalize();
    Vec3F yaxis = zaxis.cross(xaxis);

    view_mat = {
        xaxis[0], yaxis[0], zaxis[0], -xaxis.dot(eye),
        xaxis[1], yaxis[1], zaxis[1], -yaxis.dot(eye),
        xaxis[2], yaxis[2], zaxis[2], -zaxis.dot(eye),
        0.0f,     0.0f,     0.0f,     1.0f
    };
}

void Camera::render(
    const Mesh& m,
    const Vec3F& C1,
    const Vec3F& C2,
    const Mat4F& model_mat
) {
    assert(m.E.size() == m.EF.size());
    Mat4F VP_mat = proj_mat * view_mat * model_mat;

    std::vector<Vec3F> TV; // transformed V
    TV.reserve(m.V.size());
    for (const Vec4F& v : m.V) {
        Vec3F transformed = h_flatten(VP_mat * v);
        TV.push_back(transformed);
    }


    Mat3F VP_norm {
        VP_mat[0], VP_mat[1], VP_mat[2], 
        VP_mat[4], VP_mat[5], VP_mat[6], 
        VP_mat[8], VP_mat[9], VP_mat[10]
    };
    std::vector<Vec3F> TN; // transformed N
    TN.reserve(m.N.size());
    for (const Vec3F& n : m.N) {
        Vec3F transformed = VP_norm * n;
        transformed.normalize();
        TN.push_back(transformed);
    }

    Vec3F dir { view_mat[2], view_mat[6], view_mat[10] };
    for (int i = 0; i < m.E.size(); i++) {
        const Vec2I& e = m.E[i];
        const Vec2I& ef = m.EF[i];

        float ex = TV[e[0]][0];
        float ey = TV[e[0]][1];
        float sx = TV[e[1]][0];
        float sy = TV[e[1]][1];
        Vec3F dir1 = dir_vec(ex, ey);
        Vec3F dir2 = dir_vec(sx, sy);
        // right now transformed is (-1, 1) x (-1, 1)
        // screen coords are 1024 x 768
        ex *= APP_VIRTUAL_WIDTH / 2.0f;
        ey *= APP_VIRTUAL_HEIGHT / 2.0f;
        sx *= APP_VIRTUAL_WIDTH / 2.0f;
        sy *= APP_VIRTUAL_HEIGHT / 2.0f;
        ex += APP_VIRTUAL_WIDTH / 2.0f;
        ey += APP_VIRTUAL_HEIGHT / 2.0f;
        sx += APP_VIRTUAL_WIDTH / 2.0f;
        sy += APP_VIRTUAL_HEIGHT / 2.0f;
        const float thold = 0.92f;
        if (ef[1] != -1) {
            const Vec3F& n1 = TN[ef[0]];
            const Vec3F& n2 = TN[ef[1]];
            bool facing1 = n1.dot(dir1) > 0;
            bool facing2 = n1.dot(dir2) > 0;
            bool facing3 = n2.dot(dir1) > 0;
            bool facing4 = n2.dot(dir2) > 0;

            if (facing1 && facing2 && facing3 && facing4) {
                if (m.N[ef[0]].dot(m.N[ef[1]]) < thold)
                    App::DrawLine(ex, ey, sx, sy, C2[0], C2[1], C2[2]);
            } else if ((facing1 && facing2) || (facing3 && facing4)) { // edge is edge of shape
                App::DrawLine(ex, ey, sx, sy, C1[0], C1[1], C1[2]);
            }
        } else {
            App::DrawLine(ex, ey, sx, sy, C1[0], C1[1], C1[2]);
        }

    }

}

Camera::Camera(const Vec3F& eye, const Vec3F& target) {
    this->lookat(eye, target);
    this->proj_mat = AffineProj(MY_NEAR, MY_FAR, APP_VIRTUAL_HEIGHT / (float) APP_VIRTUAL_WIDTH);
}
