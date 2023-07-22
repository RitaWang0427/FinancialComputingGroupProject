#include "PriceMap.h"
#include "readData.h"
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <cmath>
#include <string>
#include <fstream>
#include <vector>
using namespace std;
using namespace fre;

typedef map<string, map<string, double>> Map;
namespace fre{
    PriceMap PriceMap::randomSelect(int n){
        Group subgroup = group.randomSelect(n);
        PriceMap subMap;
        vector<Stock> sList = subgroup.getStockList();
        for(int i = 0; i < n; i++){
            subMap.addStock(sList[i]);
            subMap.setPM(sList[i].getTicker(),getPrice(sList[i].getTicker()));
        }
        return subMap;
    }
    
    PriceMap PriceMap::createReturnMap(){
        PriceMap returnMap;
        vector<Stock> sList = getStockList();
        for(int i = 0; i < sList.size(); i++){
            Stock temp = sList[i];
            string ticker = temp.getTicker();
            returnMap.addStock(temp);
            map<string, double> price = getPrice(ticker);
            map<string, double> ret;
            double pricePrevious = price.begin()-> second;
            for(map<string,double>::iterator itr = next(price.begin()); itr!=price.end(); itr++){
                string date = itr -> first;
                double pricePresent = itr -> second;
                ret[date] = (pricePresent - pricePrevious)/(pricePrevious);
                pricePrevious = pricePresent;
            }
            returnMap.group.addStock(temp);
            returnMap.setPM(ticker,ret);
        }
        return returnMap;
    }
}
