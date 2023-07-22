#include "CalculationFunction.h"

#include <vector>
#include <cmath>
#include <numeric>
#include "Matrix.h"

using namespace std;

namespace fre{
typedef vector<double> Vector;
typedef vector<Vector> Matrix;

// Calculate Abnormal Returns
Vector calculateAR(const Vector& actualReturn, const Vector& expectedReturn) {
    Vector AR(actualReturn.size());
    AR = actualReturn - expectedReturn;
    return AR;
}

// Calculate Average Abnormal Returns
Vector calculateAverage(const Matrix& AR, int& numDays) {
    Vector avg;
    int d = 2 * numDays;
    avg.resize(d);
    
    for (int i = 0; i < d; i++) {
        avg[i] = 0.0;
    }
    
    for (int i = 0; i < AR.size(); i++) {
        avg = (i * avg + AR[i]) / (i + 1.0);
    }
    
    return avg;
}

// Calculate Cumulative Abnormal Returns
Vector calculateCAAR(const Vector& AAR){
    Vector CAAR;
    double temp = 0;
    for (int i = 0; i < AAR.size(); i++){
        temp += AAR[i];
        CAAR.push_back(temp);
    }
    return CAAR;
}

// Calculate Standard Deviation of Cumulative Abnormal Returns
Vector calculateSTD(const Matrix& caarValues) {
    int numSamples = caarValues.size();
    int numDays = caarValues[0].size();

    Vector avgCAAR = calculateAverage(caarValues, numDays);
    Vector stdDev(numDays, 0.0);

    for (int i = 0; i < numDays; i++) {
        double sumSquaredDiffs = 0.0;
        for (int j = 0; j < numSamples; j++) {
            sumSquaredDiffs += pow(caarValues[j][i] - avgCAAR[i], 2);
        }
        stdDev[i] = sqrt(sumSquaredDiffs / numSamples);
    }
    return stdDev;
}
}
