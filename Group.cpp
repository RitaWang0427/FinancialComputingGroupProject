#include "Group.h"
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <cmath>
#include <string>
#include <fstream>
#include <vector>
using namespace std;
using namespace fre;

namespace fre{
    vector<int> Group::generateRandomIndices(int n) {
        vector<int> indices(stockList.size());
        iota(indices.begin(), indices.end(), 0);
        shuffle(indices.begin(), indices.end(), std::mt19937{ std::random_device{}() });
        indices.resize(n);
        return indices;
    }
    
    void Group:: addStocksFromIndices(const vector<int>& indices, Group& group){
        for(int i = 0; i < indices.size(); i++){
            group.setStock(i,getStock(indices[i]));
        }
    }
    
    void Group::addStock(const Stock& stock) {
        stockList.push_back(stock);
        N++;
    }
    Group Group:: randomSelect(int n){
        if (n > N) {
            throw std::invalid_argument("n must be smaller or equal to the number of elements in stockList");
        }
        Group rs(n);
        vector<int> randomIndices = generateRandomIndices(n);
        addStocksFromIndices(randomIndices,rs);
        return rs;
    }
    void Group::display(){
        for (unsigned int n = 0; n < getN(); n++){
            getStock(n).display();
        }
    }
    void Group::display(ofstream& foutput) {
        foutput << "ticker,dayZero" << endl;
        for (unsigned int n = 0; n < getN(); n++) {
            Stock temp = getStock(n);
            foutput << temp.getTicker() << "," << temp.getZero() << std::endl;
     
        }
    }
}
