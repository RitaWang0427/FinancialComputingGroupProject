#pragma once
#include <vector>
#include <iostream>
using namespace std;
namespace fre {
    typedef vector<double> Vector;
    typedef vector<Vector> Matrix;
    // overload operators as independent functions
    
    Vector operator*(const int& a, const Vector& V);
    Vector operator-(const Vector& V, const Vector& W);
    Vector operator/(const Vector& V, const double& a);
    Vector operator+(const Vector& V, const Vector& W);
    ostream& operator<<(ostream& out, Vector& V);
    ostream& operator<<(ostream& out, Matrix& W);
    
}
