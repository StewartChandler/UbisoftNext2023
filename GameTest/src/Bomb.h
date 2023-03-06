#ifndef _BOMB_H
#define _BOMB_H

#include"Camera.h"
#include"Mesh.h"
#include"stdafx.h"
#include <vector>

class Bomb {
    Mat4F model;
    std::shared_ptr<Mesh> bomb;
    float fuse;
    Vec3F C1, C2;

    const Vec3F C1a{0.7f, 0.1f, 0.1f};
    const Vec3F C1b{1.0f, 0.8f, 0.8f};
    const Vec3F C2a{0.5f, 0.3f, 0.0f};
    const Vec3F C2b{1.0f, 1.0f, 0.7f};
public:
    bool active;
    Bomb (std::shared_ptr<Mesh> m, Vec3F location) :
        active(true),
        C1(C1a), 
        C2(C2a), 
        fuse(5.0f), 
        bomb(m), 
        model(AffineTranslation(location)) {}

    void set(Vec3F location) {
        model = AffineTranslation(location);
        active = true;
        fuse = 5.0f;
    }

    void update(float deltatime) {
        if (active) {
            fuse -= deltatime;
            if (fuse < 0) {
                active = false;
            } else {
                const float x = fuse / 5;
                // smoothstep
                const float b = 3 * x * x - 2 * x * x * x;
                const float a = 1 - b;

                C1 = C1a * a + C1b * b;
                C2 = C2a * a + C2b * b;
            }
        }
    }

    void render(Camera& c) {
        if (active) {
            c.render(*bomb, C1, C2, model);
        }
    }
};

#endif
