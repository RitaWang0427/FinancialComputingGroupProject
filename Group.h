#pragma once
#include <iostream>
#include "Stock.h"
#include <random>
#include <vector>
#include <algorithm>
using namespace std;

namespace fre{
    class Group{
        private:
        int N;
        vector<Stock> stockList;
        
        vector<int> generateRandomIndices(int n);
        void addStocksFromIndices(const vector<int>& indices, Group& group);
        
        
        public:
        Group(): N(0){}
        Group(int N_): N(N_){ stockList.resize(N+1);}
        Group(const Group& g) : N(g.N), stockList(g.stockList) {}
        ~Group(){}
        void SetN(int N_){
            N = N_;
            stockList.resize(N+1);
        }
        
        void setStock(int i, Stock s){
                stockList[i] = s;
        }
        vector<Stock> getStockList() const {return stockList;}
        Stock getStock(int i) const { return stockList[i]; }
        int getN() const {return N;}
        void addStock(const Stock& stock);
        void deleteStock(const Stock& s) {
            stockList.erase(
                std::remove_if(stockList.begin(), stockList.end(), [&](const Stock& stock) {
                    return stock.getTicker() == s.getTicker();
                }),
                stockList.end()
            );
        }
        Group randomSelect(int n);
        
        void display();
        void display(std::ofstream& foutput);
    };
}
