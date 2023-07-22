#pragma once
#include<vector>
#include<string>
#include<map>
#include"Group.h"
#include "PriceMap.h"
#include "CalculationFunction.h"
typedef vector<double> Vector;
typedef vector<Vector> Matrix;

namespace fre{
    Matrix Bootstrap(PriceMap& group, int numDays, int sampleSize, map<string, double>& benchmark);
    void displayGroupResult(Matrix& groupResult);
    vector<Matrix> BootstrapResult(PriceMap& groupbeat, PriceMap& groupmiss, PriceMap& groupmeet, int numDays, int sampleSize, map<string, double>& benchmark);
    void displayBootstrapResult(vector<Matrix>& boostrapResult);
}
