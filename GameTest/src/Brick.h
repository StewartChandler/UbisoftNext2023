#ifndef _BRICK_H
#define _BRICK_H

#include"Camera.h"
#include"Mesh.h"
#include"stdafx.h"
#include <vector>

class Brick {
    Mat4F model;
    std::shared_ptr<Mesh> brick;
public:
    bool active = true;
    Brick (std::shared_ptr<Mesh> m, Vec3F location) : brick(m), model(AffineTranslation(location)) {}

    void render(Camera& c) {
        if (active) {
            c.render(*brick, {0.9f, 0.7f, 0.6f}, {0.4f, 0.2f, 0.1f}, model);
        }
    }
};

#endif
