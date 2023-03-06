#ifndef _DRAW_BUF_H
#define _DRAW_BUF_H
#include<vector>

#include"Vec.h"

class DrawBuf
{
public:
    std::vector<Vec4F> V; // list of vertices (homogeneous coords)
    std::vector<Vec3I> F; // list of faces
    std::vector<Vec3F> N; // list of face normals (len N == len F)
    std::vector<Vec2I> E; // list of edges
    std::vector<Vec2I> EF; // adjacent faces of edges (len E == len EF)
    std::vector<Vec3F> TV; // transformed vertices (len E == len EF)
    std::vector<Vec3F> TN; // transformed normals (len E == len EF)
};

#endif