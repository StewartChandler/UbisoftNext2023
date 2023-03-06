#ifndef _VEC_H
#define _VEC_H
#include <utility>
#include <cassert>
#include <type_traits>

template <const int N, typename T>
class Vec {
private:
    T inner[N];
public:
    struct VecHasher {
        std::size_t operator()(const Vec<N, T>& v) const {
            std::size_t val = std::hash<T>()(v[0]);
            for (int i = 1; i < N; i++) 
                val ^= std::hash<T>()(v[i]);
            return val;
        }
    };

    Vec() = default;
    Vec(const Vec<N, T>& other) = default;
    Vec<N, T>& operator=(const Vec<N, T>& other) = default;
    Vec(Vec<N, T>&& other) = default;
    
    Vec(std::initializer_list<T> l) {
        // static_assert(l.size() == N * N, "Vector must be initialized with N members");
        assert(l.size() == N || l.size() == 1);
        if (l.size() == N)
            std::copy(l.begin(), l.end(), inner);
        else
            std::fill_n(inner, N, *l.begin());
    }

    T& operator[](std::size_t index) {
        return inner[index];
    }
    const T& operator[](std::size_t index) const {
        return inner[index];
    }

    // lexicographic comparison
    bool operator==(const Vec<N, T>& other) const{
        for (int i = 0; i < N; i++)
            if ((*this)[i] != other[i]) return false;
        return true;
    }
    bool operator!=(const Vec<N, T>& other) const { return !(*this == other); };
    bool Vec<N, T>::operator <(const Vec<N, T> &other) const {
        for (int i = 0; i < N; i++)
            if (inner[i] < other.inner[i])
                return true;
            else if (inner[i] > other.inner[i])
                return false;
        return false;
    }
    bool Vec<N, T>::operator >(const Vec<N, T> &other) const { return other < *this; }
    bool Vec<N, T>::operator <=(const Vec<N, T> &other) const { return !(other < *this); }
    bool Vec<N, T>::operator >=(const Vec<N, T> &other) const { return !(other > *this); }

    // math operators (unary)
    Vec<N, T> operator+() const {
        Vec<N, T> result;
        for(int i = 0; i < N; i++)
            result[i] = +(*this)[i];        
        return result;    
    }
    Vec<N, T> operator-() const {
        Vec<N, T> result;
        for(int i = 0; i < N; i++)
            result[i] = -(*this)[i];        
        return result;    
    }

    // math operators (elementwise):
    Vec<N, T> operator+(const Vec<N, T>& other) const {
        Vec<N, T> result;
        for(int i = 0; i < N; i++)
            result[i] = (*this)[i] + other[i];        
        return result;    
    }
    Vec<N, T> operator-(const Vec<N, T>& other) const {
        Vec<N, T> result;
        for(int i = 0; i < N; i++)
            result[i] = (*this)[i] - other[i];        
        return result;    
    }
    Vec<N, T> operator*(const Vec<N, T>& other) const {
        Vec<N, T> result;
        for(int i = 0; i < N; i++)
            result[i] = (*this)[i] * other[i];        
        return result;    
    }
    Vec<N, T> operator/(const Vec<N, T>& other) const {
        Vec<N, T> result;
        for(int i = 0; i < N; i++)
            result[i] = (*this)[i] / other[i];        
        return result;    
    }

    // math operators (scalar):
    Vec<N, T> operator+(const T& other) const {
        Vec<N, T> result;
        for(int i = 0; i < N; i++)
            result[i] = (*this)[i] + other;        
        return result;    
    }
    Vec<N, T> operator-(const T& other) const {
        Vec<N, T> result;
        for(int i = 0; i < N; i++)
            result[i] = (*this)[i] - other;        
        return result;    
    }
    Vec<N, T> operator*(const T& other) const {
        Vec<N, T> result;
        for(int i = 0; i < N; i++)
            result[i] = (*this)[i] * other;        
        return result;    
    }
    Vec<N, T> operator/(const T& other) const {
        Vec<N, T> result;
        for(int i = 0; i < N; i++)
            result[i] = (*this)[i] / other;    
        return result;    
    }

    // assignment operators:
    Vec<N, T>& operator+=(const Vec<N, T>& other) { *this = *this + other; return *this; }
    Vec<N, T>& operator-=(const Vec<N, T>& other) { *this = *this - other; return *this; }
    Vec<N, T>& operator*=(const Vec<N, T>& other) { *this = *this * other; return *this; }
    Vec<N, T>& operator/=(const Vec<N, T>& other) { *this = *this / other; return *this; }
    Vec<N, T>& operator+=(const T& other) { *this = *this + other; return *this; }
    Vec<N, T>& operator-=(const T& other) { *this = *this - other; return *this; }
    Vec<N, T>& operator*=(const T& other) { *this = *this * other; return *this; }
    Vec<N, T>& operator/=(const T& other) { *this = *this / other; return *this; }

    Vec<N, T>& normalize() {
        static_assert(std::is_same<T, float>::value, "normalized only available for floats");
        float norm = 0.0f;
        for (int i = 0; i < N; i++)
            norm += this->inner[i] * this->inner[i];
        
        norm = std::sqrt(norm);
        *this /= norm;
        return *this;
    }
    Vec<N, T> cross(const Vec<N, T>& other) {
        static_assert(N == 3, "cross only available for N = 3");
        return Vec<N, T> {
            (*this)[1] * other[2] - (*this)[2] * other[1],
            (*this)[2] * other[0] - (*this)[0] * other[2],
            (*this)[0] * other[1] - (*this)[1] * other[0],
        };
    }
    T dot(const Vec<N, T>& other) const {
        T dotprod = 0;
        for (int i = 0; i < N; i++)
            dotprod += (*this)[i] * other[i];
        return dotprod;
    }
};


using Vec4F = Vec<4, float>;
using Vec3F = Vec<3, float>;
using Vec2F = Vec<2, float>;
using Vec3I = Vec<3, int>;
using Vec2I = Vec<2, int>;
using HashVec4F = Vec<4, float>::VecHasher;
using HashVec3F = Vec<3, float>::VecHasher;
using HashVec3I = Vec<3, int>::VecHasher;
using HashVec2I = Vec<2, int>::VecHasher;

// normalizes a vector in homogeneous coordinates
Vec3F h_normalize(const Vec4F& other);
// cross product, in homogeneous coordinates
Vec4F h_cross(const Vec4F& a, const Vec4F& b);
// subtraction, in homogeneous coordinates
Vec4F h_sub(const Vec4F& a, const Vec4F& b);
// converts vector in homogeneous coordinates to carteasian coords
Vec3F h_flatten(const Vec4F& a);

#endif