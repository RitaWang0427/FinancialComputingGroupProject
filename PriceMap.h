#pragma once
#include <iostream>
#include "Stock.h"
#include "Group.h"
#include "readData.h"
#include <random>
#include <vector>
#include <thread>
#include <mutex>
using namespace std;
typedef map<string, map<string, double>> Map;
namespace fre{
    class PriceMap{
        private:
        vector<Stock> stockList;
        Group group;
        int numDays;
        Map pm;
        
        public:
        PriceMap(){}
        PriceMap(const Group& g, int nDays) : group(g), numDays(nDays) {
            int num_threads = 2;
            std::vector<std::thread> threads;
            std::mutex mutex;

            auto worker = [&](int start, int end) {
                for (int n = start; n < end; ++n) {
                    Stock temp = g.getStock(n);
                    map<string, double> value = readValue(temp, nDays);
                    if (value.empty()) {
                        group.deleteStock(temp);
                        continue;
                    }
                    std::unique_lock<std::mutex> lock(mutex);
                    stockList.push_back(temp);
                    pm[temp.getTicker()] = value;
                    lock.unlock();
                }
            };

            int n = g.getN();
            int step = (n + num_threads - 1) / num_threads;

            for (int i = 0; i < num_threads; ++i) {
                int start = i * step;
                int end = std::min(start + step, n);
                threads.emplace_back(worker, start, end);
            }

            for (auto& t : threads) {
             t.join();
            }
        }
        ~PriceMap(){}
        vector<Stock> getStockList() const {return stockList;}
        Map getPM() const {return pm;}
        map<string, double> getPrice(string ticker) {return pm[ticker];}
        void setStockList(vector<Stock> sList) {stockList = sList;}
        void setPM(string ticker, map<string, double> price){
            pm[ticker] = price;
        }
        void addStock(Stock s) {stockList.push_back(s);}
        PriceMap randomSelect(int n);
        PriceMap createReturnMap();
    };
}
