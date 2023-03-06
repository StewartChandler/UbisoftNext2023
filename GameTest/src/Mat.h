#ifndef _MAT_H
#define _MAT_H
#include "Vec.h"

template <const int N, typename T>
class Mat {
public:
    T inner[N * N];

    Mat() = default;
    Mat(const Mat<N, T>& other) = default;
    Mat<N, T>& operator=(const Mat<N, T>& other) = default;
    Mat(Mat<N, T>&& other) = default;
    
    Mat(std::initializer_list<T> l) {
        // constexpr l.size() not introduced until c++14 and the compiler is compileing with c++11
        // right now, I don't know if I'm allowed to change it so, instead this will be a normal 
        // assert.
        // static_assert(l.size() == N * N, "Matrix must be initialized with N * N members");
        assert(l.size() == N * N || l.size() == 1);
        if (l.size() == N * N)
            std::copy(l.begin(), l.end(), inner);
        else
            std::fill_n(inner, N * N, *l.begin());
    }

    T& operator[](std::size_t index) {
        return inner[index];
    }
    const T& operator[](std::size_t index) const {
        return inner[index];
    }

    // lexicographic comparison
    bool operator==(const Mat<N, T>& other) const{
        for (int i = 0; i < N; i++)
            if ((*this)[i] != other[i]) return false;
        return true;
    }
    bool operator!=(const Mat<N, T>& other) const { return !(*this == other); };
    bool Mat<N, T>::operator <(const Mat<N, T> &other) const {
        for (int i = 0; i < N; i++)
            if (inner[i] < other.inner[i])
                return true;
            else if (inner[i] > other.inner[i])
                return false;
        return false;
    }
    bool Mat<N, T>::operator >(const Mat<N, T> &other) const { return other < *this; }
    bool Mat<N, T>::operator <=(const Mat<N, T> &other) const { return !(other < *this); }
    bool Mat<N, T>::operator >=(const Mat<N, T> &other) const { return !(other > *this); }

    // math operator: matrix multiply
    Mat<N, T> operator*(const Mat<N, T>& other) const {
        Mat<N, T> result { 0.0f };
        for(int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
                for (int k = 0; k < N; k++)
                    result[i * N + j] += (*this)[i * N + k] * other[k * N + j];        
        return result;    
    }
    Vec<N, T> operator*(const Vec<N, T>& other) const {
        Vec<N, T> result {0.0f};
        for (int j = 0; j < N; j++)
            for (int k = 0; k < N; k++)
                result[j] += (*this)[j * N + k] * other[k];        
        return result;    
    }


    // math operators (unary)
    Mat<N, T> operator+() const {
        Mat<N, T> result;
        for(int i = 0; i < N; i++)
            result[i] = +(*this)[i];        
        return result;    
    }
    Mat<N, T> operator-() const {
        Mat<N, T> result;
        for(int i = 0; i < N; i++)
            result[i] = -(*this)[i];        
        return result;    
    }

    // math operators (elementwise):
    Mat<N, T> operator+(const Mat<N, T>& other) const {
        Mat<N, T> result;
        for(int i = 0; i < N; i++)
            result[i] = (*this)[i] + other[i];        
        return result;    
    }
    Mat<N, T> operator-(const Mat<N, T>& other) const {
        Mat<N, T> result;
        for(int i = 0; i < N; i++)
            result[i] = (*this)[i] - other[i];        
        return result;    
    }
    Mat<N, T> operator/(const Mat<N, T>& other) const {
        Mat<N, T> result;
        for(int i = 0; i < N; i++)
            result[i] = (*this)[i] / other[i];        
        return result;    
    }

    // math operators (scalar):
    Mat<N, T> operator+(const T& other) const {
        Mat<N, T> result;
        for(int i = 0; i < N; i++)
            result[i] = (*this)[i] + other;        
        return result;    
    }
    Mat<N, T> operator-(const T& other) const {
        Mat<N, T> result;
        for(int i = 0; i < N; i++)
            result[i] = (*this)[i] - other;        
        return result;    
    }
    Mat<N, T> operator*(const T& other) const {
        Mat<N, T> result;
        for(int i = 0; i < N; i++)
            result[i] = (*this)[i] * other;        
        return result;    
    }
    Mat<N, T> operator/(const T& other) const {
        Mat<N, T> result;
        for(int i = 0; i < N; i++)
            result[i] = (*this)[i] / other;    
        return result;    
    }

    // assignment operators:
    Mat<N, T>& operator*=(const Mat<N, T>& other) { *this = *this * other; return *this; }
    Vec<N, T>& operator*=(const Vec<N, T>& other) { *this = *this * other; return *this; }
    Mat<N, T>& operator+=(const Mat<N, T>& other) { *this = *this + other; return *this; }
    Mat<N, T>& operator-=(const Mat<N, T>& other) { *this = *this - other; return *this; }
    Mat<N, T>& operator/=(const Mat<N, T>& other) { *this = *this / other; return *this; }
    Mat<N, T>& operator+=(const T& other) { *this = *this + other; return *this; }
    Mat<N, T>& operator-=(const T& other) { *this = *this - other; return *this; }
    Mat<N, T>& operator*=(const T& other) { *this = *this * other; return *this; }
    Mat<N, T>& operator/=(const T& other) { *this = *this / other; return *this; }

};

using Mat4F = Mat<4, float>;
using Mat3F = Mat<3, float>;

Mat4F AffineProj(
    const float near,
    const float far,
    const float aspect
);

Mat4F AffineTranslation(const Vec3F& trans);
Mat4F AffineRotation(const float phi);
#endif
