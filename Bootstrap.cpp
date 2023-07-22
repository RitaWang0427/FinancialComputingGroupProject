#include <iostream>
#include <map>
#include <vector>
#include <iomanip>
#include "Group.h"
#include "CalculationFunction.h"
#include "readData.h"
#include "Bootstrap.h"
#include "PriceMap.h"

using namespace std;
using namespace fre;

typedef vector<double> Vector;
typedef vector<Vector> Matrix;

namespace fre{
Matrix Bootstrap(PriceMap& returnMap, int numDays, int sampleSize, map<string, double>& etfReturns) {

    Matrix groupResult;
    Matrix bootstrapAAR;
    Matrix bootstrapCAAR;

    for (int i = 0; i < 40; i++) {
        //Group boostrapGroup = group.randomSelect(sampleSize);
        //map<string, map<string, double>> priceMapGroup = createPriceMap(boostrapGroup, numDays);
        PriceMap random = returnMap.randomSelect(sampleSize);
        map<string, map<string, double>> returnMapGroup = random.getPM();
        
        
        Matrix returnGroupMatrix;

        for (map<string, map<string, double>>::iterator itr = returnMapGroup.begin(); itr != returnMapGroup.end(); itr++) {

            map<string, double> temp = itr->second;
            vector<double> returns;
            vector<double> etfReturn;
            vector<string> date;

            for (map<string, double>::iterator itr2 = temp.begin(); itr2 != temp.end(); itr2++ ){
                date.push_back(itr2->first);
                returns.push_back(itr2->second);
            }

            for (int i = 0; i < date.size(); i++){
                etfReturn.push_back(etfReturns[date[i]]);
            }

            Vector AR = calculateAR(returns, etfReturn);
            returnGroupMatrix.push_back(AR);
        }

        Vector averageAR = calculateAverage(returnGroupMatrix, numDays);
        Vector cumulateAR = calculateCAAR(averageAR);

        bootstrapAAR.push_back(averageAR);
        bootstrapCAAR.push_back(cumulateAR);
    }

    vector<double> averageAAR = calculateAverage(bootstrapAAR, numDays);
    vector<double> averageCAAR = calculateAverage(bootstrapCAAR, numDays);
    vector<double> stdDevAAR = calculateSTD(bootstrapAAR);
    vector<double> stdDevCAAR = calculateSTD(bootstrapCAAR);

    groupResult.push_back(averageAAR);
    groupResult.push_back(averageCAAR);
    groupResult.push_back(stdDevAAR);
    groupResult.push_back(stdDevCAAR);

    return groupResult;
}

vector<Matrix> BootstrapResult(PriceMap& groupbeat, PriceMap& groupmiss, PriceMap& groupmeet, int numDays, int sampleSize, map<string, double>& benchmark){
    
    Matrix beatresult, missresult, meetresult;
    vector<Matrix> boostrapResult;
    
    beatresult = Bootstrap(groupbeat, numDays, sampleSize, benchmark);
    missresult = Bootstrap(groupmiss, numDays, sampleSize, benchmark);
    meetresult = Bootstrap(groupmeet, numDays, sampleSize, benchmark);
    
    boostrapResult.push_back(beatresult);
    boostrapResult.push_back(missresult);
    boostrapResult.push_back(meetresult);
    
    return boostrapResult;
}

void displayGroupResult(Matrix& groupResult){
    
    cout << "------------------------ Average AAR -------------------------" << endl;
    int count_1 = 0;
    for (int i = 0; i < groupResult[0].size(); i++){
        cout << setw(14) << groupResult[0][i] << " ";
        count_1++;
        if (count_1 % 10 == 0) cout << endl;
    }
    cout << endl;
    cout << endl;
    
    cout << "------------------------ Average CAAR -------------------------" << endl;
    int count_2 = 0;
    for (int i = 0; i < groupResult[1].size(); i++){
        cout << setw(14) << groupResult[1][i] << " ";
        count_2++;
        if (count_2 % 10 == 0) cout << endl;
    }
    cout << endl;
    cout << endl;
    
    cout << "------------------- AAR Standard Deviation --------------------" << endl;
    int count_3 = 0;
    for (int i = 0; i < groupResult[2].size(); i++){
        cout << setw(14) << groupResult[2][i] << " ";
        count_3++;
        if (count_3 % 10 == 0) cout << endl;
    }
    cout << endl;
    cout << endl;
    
    cout << "------------------- CAAR Standard Deviation --------------------" << endl;
    int count_4 = 0;
    for (int i = 0; i < groupResult[3].size(); i++){
        cout<< setw(14) << groupResult[3][i] << " ";
        count_4++;
        if (count_4 % 10 == 0) cout << endl;
    }
    cout << endl;
    cout << endl;
}

void displayBootstrapResult(vector<Matrix>& boostrapResult){
    
    cout << "----- Beat -----" << endl;
    cout << endl;
    displayGroupResult(boostrapResult[0]);
    cout << endl;
    
    cout << "----- Miss -----" << endl;
    cout << endl;
    displayGroupResult(boostrapResult[1]);
    cout << endl;
    
    cout << "----- Meet -----" << endl;
    cout << endl;
    displayGroupResult(boostrapResult[2]);
    cout << endl;
}


}
