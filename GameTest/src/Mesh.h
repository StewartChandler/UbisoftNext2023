#ifndef _MESH_H
#define _MESH_H
#include<vector>
#include<string>

#include "Vec.h"

class Mesh {
private:
    void read_from_obj(const std::string fpath);
    void calc_N(const std::vector<Vec3F>& VN, const std::vector<Vec3I>& NI);
    void calc_E();
public:
    std::vector<Vec4F> V; // list of vertices (homogeneous coords)
    std::vector<Vec3I> F; // list of faces
    std::vector<Vec3F> N; // list of face normals (len N == len F)
    std::vector<Vec2I> E; // list of edges
    std::vector<Vec2I> EF; // adjacent faces of edges (len E == len EF)

    // Mesh& operator<<(Mesh& other) {
    //     int preV = this->V.size();
    //     int preF = this->F.size();
    //     this->V.insert( this->V.end(), other.V.begin(), other.V.end() );
    //     this->N.insert( this->N.end(), other.N.begin(), other.N.end() );
    //     this->F.insert( this->F.end(), other.F.begin(), other.F.end() );
    //     this->E.insert( this->E.end(), other.E.begin(), other.E.end() );
    //     this->EF.insert( this->EF.end(), other.EF.begin(), other.EF.end() );
    //     for (int i = preV; i < this->F.size(); i++) 
    //         this->F[i] += preV; 
    //     for (int i = preV; i < this->E.size(); i++) 
    //         this->E[i] += preV; 
    //     for (int i = preF; i < this->EF.size(); i++)
    //         this->EF[i] += preF; 
    //     return *this;
    // }

    // void translate(const Vec3F& translation) {
    //     // Mesh result;
    //     Vec4F temp {translation[0],translation[1],translation[2],0.0f};
    //     for (Vec4F& v : V) {
    //         v += temp * v[3];
    //     }
    // }

    Mesh(const std::string fpath);
    Mesh() {}
    int num_faces() const { return F.size(); }
    int num_verts() const { return V.size(); }
    int num_edges() const { return E.size(); }
};

#endif