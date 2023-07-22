#include <vector>
#include "Matrix.h"
using namespace std;

namespace fre{
typedef vector<double> Vector;
typedef vector<Vector> Matrix;

Vector calculateAR(const Vector& actualReturn, const Vector& expectedReturn);
Vector calculateAverage(const Matrix& arValues, int& numDays);
Vector calculateCAAR(const Vector& AvgAR);
Vector calculateSTD(const Matrix& caarValues);

}
