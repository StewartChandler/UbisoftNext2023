#include "stdafx.h"
#include "Mesh.h"

void Mesh::read_from_obj(const std::string fpath) {
    std::string line, token;
    std::ifstream infile (fpath);
    std::stringstream linestream;
    if (!infile.is_open()) {
        perror("Error open");
        exit(EXIT_FAILURE);
    }

    std::vector<Vec3F> VN;
    std::vector<Vec3I> NI;

    while(getline(infile, line)) {
        int end = line.find_first_of('#');
        linestream = std::stringstream(line.substr(0, end));
        linestream >> std::ws; // skip whitespace
        
        if (!getline(linestream, token, ' ')) { // nothing on this line
            continue;
        }
        
        if (token == "f") {
            F.emplace_back();
            NI.emplace_back();
            Vec3I& added = F.back();
            Vec3I& added_n = NI.back();
            added_n = { -1 };
            for (int i = 0; i < 3; i++) {
                if (!getline(linestream >> std::ws, token, ' ')) {
                    std::cerr << "insufficieent number of values for `f`\n";
                    exit(-1);
                }
                std::size_t pos = token.find_first_of('/');
                added[i] = std::stoi(token.substr(0, pos)) - 1; // obj uses 1-indexing so we need to decrement
                if (pos != std::string::npos) { // included normals
                    added_n[i] = std::stoi(token.substr(token.find_last_of('/') + 1)) - 1;
                }
            }
        } else if (token == "v") {
            V.emplace_back();
            Vec4F& added = V.back();
            for (int i = 0; i < 3; i++) {
                if (!getline(linestream >> std::ws, token, ' ')) {
                    std::cerr << "insufficieent number of values for `v`\n";
                    exit(-1);
                }

                added[i] = std::stof(token);
            }

            // last value is optional in the spec
            if (!getline(linestream >> std::ws, token, ' ')) {
                added[3] = 1.0f;
            } else {
                added[3] = std::stof(token);
            }
        } else if (token == "vn") {
            VN.emplace_back();
            Vec3F& added = VN.back();
            for (int i = 0; i < 3; i++) {
                if (!getline(linestream >> std::ws, token, ' ')) {
                    std::cerr << "insufficieent number of values for `vn`\n";
                    exit(-1);
                }

                added[i] = std::stof(token);
            }

        }
    }

    calc_N(VN, NI);
    infile.close();
}

void Mesh::calc_N(const std::vector<Vec3F>& VN, const std::vector<Vec3I>& NI) {
    const int num = num_faces();
    N.resize(num);
    for (int i = 0; i < num; i++) {
        N[i] = h_normalize(h_cross(h_sub(V[F[i][1]], V[F[i][0]]), h_sub(V[F[i][2]], V[F[i][1]])));
        Vec3F temp{0.0f};
        
        for (int j = 0; j < 3; j++) {
            if (NI[i][j] != -1) {
                temp += VN[NI[i][j]];
            }
        }

        if (N[i].dot(temp) < 0) {
            N[i] = -N[i];
        }
    }
}

void Mesh::calc_E() {
    const int num = num_faces();
    std::unordered_map<Vec2I, int, HashVec2I> edge_id;
    E.clear();
    EF.clear();
    // we make the assumption that an egde has 1 or 2 adjacent faces
    for (int i = 0; i < num; i++) {
        for (int j = 0; j < 3; j++) {
            Vec2I edge;
            edge[0] = F[i][j]; 
            edge[1] = F[i][(j + 1) % 3];
            if (edge[1] < edge[0]) std::swap(edge[0], edge[1]);

            auto id = edge_id.find(edge);
            
            if (id == edge_id.end()) {
                E.push_back(edge);
                EF.emplace_back();
                EF[E.size() - 1][0] = i;
                EF[E.size() - 1][1] = -1;
                edge_id.insert({edge, E.size() - 1});
            } else { // edge has already bee added
                EF[id->second][1] = i;
            }
        }
    }
}

// only supports a subset of the spec
Mesh::Mesh(const std::string fpath) {
    std::ofstream log_out ("out.txt");
    // log_out << "mesh ctor\n";
    read_from_obj(fpath);
    
    // log_out << "verts\n";

    // for (Vec4F& v : V) {
    //     log_out << "  " << v[0] << ", " << v[1] << ", " << v[2] << ", " << v[3] << std::endl;
    // }

    // log_out << "faces\n";

    // for (Vec3I& f : F) {
    //     log_out << "  " << f[0] << ", " << f[1] << ", " << f[2] << std::endl;
    // }
    // calc_N();
    log_out << "normals\n";
    for (Vec3F& n : N)
        log_out << "  " << n[0] << ", " << n[1] << ", " << n[2] << std::endl;

    calc_E();
    log_out << "edges\n";
    for (Vec2I& e : E)
        log_out << "  " << e[0] << ", " << e[1] << std::endl;
    log_out << "edge adj faces\n";
    for (Vec2I& ef : EF)
        log_out << "  " << ef[0] << ", " << ef[1] << std::endl;
    log_out.close();
}