#include <iostream>
#include <iomanip>
#include <fstream>
#include <map>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include "Stock.h"
#include "Group.h"
#include "sortToGroup.h"
#include "readData.h"
#include "Bootstrap.h"
#include "visualization.h"
#include "CalculationFunction.h"
#include "PriceMap.h"

using namespace std;
using namespace fre;

std::vector<PriceMap> pmVector;
std::mutex mtx;

Stock findTickerFromLists(const vector<Stock>& beatlist, const vector<Stock>& meetlist, const vector<Stock>& misslist, const string& tickerToFind, 
vector<string> &price_dates, vector <double> &price_dp, PriceMap & beatMap,PriceMap &meetMap, PriceMap&missMap) {
    // Search in beatlist
    map <string, double> search;
    for (const auto& stock : beatlist) {
        if (stock.getTicker() == tickerToFind) {
            cout << "Group: Beat" << endl;
            search = beatMap.getPrice(tickerToFind);
            for (map<string,double> :: iterator itr = search.begin();itr != search.end(); itr ++)
            {
                price_dates.push_back(itr -> first);
                price_dp.push_back(itr -> second);
            }
            return stock;
        }
    }

    // Search in meetlist
    for (const auto& stock : meetlist) {
        if (stock.getTicker() == tickerToFind) {
            cout << "Group: Meet" << endl;
            search = meetMap.getPrice(tickerToFind);
            for (map<string,double> :: iterator itr = search.begin();itr != search.end(); itr ++)
            {
                price_dates.push_back(itr -> first);
                price_dp.push_back(itr -> second);
            }
            return stock;
        }
    }

    // Search in misslist
    for (const auto& stock : misslist) {
        if (stock.getTicker() == tickerToFind) {
            cout << "Group: Miss" << endl;
            search = missMap.getPrice(tickerToFind);
            for (map<string,double> :: iterator itr = search.begin();itr != search.end(); itr ++)
            {
                price_dates.push_back(itr -> first);
                price_dp.push_back(itr -> second);
            }
            return stock;
        }
    }
    throw runtime_error("Sorry, this stock cannot be located in any group, possibly due to insufficient data available.");
    
}

void Clac_DR(Vector &price_dp, Vector &price_dr){
    for (int i = 0; i<price_dp.size()-1; i++){
        price_dr.push_back((price_dp[i+1] - price_dp[i])/price_dp[i]);
    }
}

map<string, double> returnBenchMark(map<string, double>& price){
    map<string,double> ret;
    double pricePrevious = price.begin()-> second;
    for(map<string,double>::iterator itr = next(price.begin()); itr!=price.end(); itr++){
        string date = itr -> first;
        double pricePresent = itr -> second;
        ret[date] = (pricePresent - pricePrevious)/(pricePrevious);
        pricePrevious = pricePresent;
    }
    return ret;
}

void createPriceMap(vector<Group>& result, int numDays, int i) {
    PriceMap pm(result[i], numDays);
    std::unique_lock<std::mutex> lock(mtx);
    pmVector.push_back(pm);
    lock.unlock();
}


int main(){
    int num_threads = 3;
    std::vector<std::thread> threads;
    
    cout << "--------------------------------INITIALIZATION--------------------------------" << endl << endl;
    cout << "[Initialize the number N (>=60)]" << endl << endl;
    
    int numDays;
    vector<Group> result = getGroup();
    Group beat = result[0];
    Group miss = result[1];
    Group meet = result[2];
    
    bool running = true;
    string option;
    string sortGroup;
    string ticker;
    
    Stock etf("IWV","2022-11-09","",1.2,0.0,0.0,0.0);
    map<string, double>  benchmark = readValue(etf, 120);
    map<string, double> etfReturn = returnBenchMark(benchmark);
    
    PriceMap beatMap, missMap, meetMap;
    PriceMap returnBeat, returnMiss, returnMeet;
    vector<Stock> beatlist, meetlist, misslist;
    int sampleSize = 80;
    vector <Matrix> bootstrapResult;
    while (running) {
        cout << endl;   
        cout << "-------------------------------------MENU-------------------------------------" << endl << endl;
        cout << "1. Enter N to retrieve 2N+1 days of historical data for all stocks (N must be larger or equal to 60)" << endl;
        cout << "2. Pull infomation of a single stock by entering the ticker" << endl;
        cout << "3. Show AAR, AAR-STD, CAAR and CAAR-STD for one group" << endl;
        cout << "4. Show the gnuplot graph with CAAR for all 3 groups" << endl;
        cout << "5. Exit" << endl << endl;
        cout << "Please Enter Number 1-5: ";
        cin >> option;
        getchar();

        cout << endl;
        cout << "-----------------------------------FUNCTION-----------------------------------" << endl << endl;
    
    
        while ((option.at(0) > '5' || option.at(0) <= '0')) {
                cout << "------------------------------------REMINDER----------------------------------" << endl << endl;
                cout << "Invalid option. Please input a number from 1 to 5" << endl;
                cout << "1. Enter N to retrieve 2N+1 days of historical data for all stocks (N must be larger or equal to 60)" << endl;
                cout << "2. Pull infomation of a single stock by entering the tickerk" << endl;
                cout << "3. Show AAR, AAR-STD, CAAR and CAAR-STD for one group" << endl;
                cout << "4. Show the gnuplot graph with CAAR for all 3 groups" << endl;
                cout << "5. Exit" << endl << endl;
                cout << "Please Enter Number 1-5: ";
                cin >> option;
                getchar();
                cout << endl;
                cout << "-----------------------------------FUNCTION-----------------------------------" << endl << endl;
            }
            
        int opt = stoi(option);
        switch (opt) {
            
            case 1:{
                cout << "Enter N to retrieve 2N+1 days of historical data for all stocks (N must be larger or equal to 60)" << endl << endl;
                cin >> numDays;
                
                for (int i = 0; i < num_threads; ++i) {
                    threads.emplace_back(createPriceMap, std::ref(result), numDays, i);
                }
                for (auto& t : threads) {
                    t.join();
                }
                beatMap = pmVector[1];
                missMap = pmVector[2];
                meetMap = pmVector[0];
                
                returnBeat = beatMap.createReturnMap();
                returnMiss = missMap.createReturnMap();
                returnMeet = meetMap.createReturnMap();
                break;
            }
            
            case 2: {
                vector <string> price_dates;
                vector <double> price_dp, price_dr, price_cdr;
                cout << "Pull infomation of a single stock by entering the ticker" << endl << endl;
                cout << "Please enter stock ticker (print '0' to get back):";
                cin >> ticker;
                getchar();
                cout << endl;
                
                cout << "Ticker Name: " << ticker << endl;
                beatlist = returnBeat.getStockList();
                meetlist = returnMeet.getStockList();
                misslist = returnMiss.getStockList();
                
                Stock enteredStock;
                try{
                    enteredStock = findTickerFromLists(beatlist, meetlist, misslist, ticker, price_dates, price_dp, beatMap, meetMap, missMap);
                }catch(const std::exception &e) {
                    break;
                }
                
                Clac_DR(price_dp, price_dr);
                price_cdr = calculateCAAR(price_dr);
                
                cout << "Earning Period Ending: " << enteredStock.getPeriodEnding() << endl;
                cout << "Earning Estimate: " << enteredStock.getEstimate() << endl;
                cout << "Reported Earnings per Share: " << enteredStock.getReported() << endl;
                cout << "Earnings Surprise $: " << enteredStock.getSurprise() << endl;
                cout << "Earnings Surprise Percentage %: " << enteredStock.getSurprisePercentage() << endl;
                cout << "Stock Daily Price Dates: " << endl;
                
                int COUNT_1 = 0;
                for (int i = 1; i < price_dates.size(); i++){
                    cout << setw(14) << price_dates[i] << " ";
                    COUNT_1++;
                    if (COUNT_1 % 10 == 0) cout << endl;
                }
                cout << endl << endl;
                
                int COUNT_2 = 0;
                cout << "Stock Daily Price: " << endl;
                for (int i = 1; i < price_dp.size(); i++){
                    cout << setw(14) << price_dp[i] << " ";
                    COUNT_2++;
                    if (COUNT_2 % 10 == 0) cout << endl;
                }
                cout << endl << endl;
                
                int COUNT_3 = 0;
                cout << "Stock Culmulative Daily Returns" << endl;
                for (int i = 0; i < price_cdr.size(); i++){
                    cout << setw(14) << price_cdr[i] << " ";
                    COUNT_3++;
                    if (COUNT_3 % 10 == 0) cout << endl;
                }
                cout << endl << endl;
                
            }
            
            case 3: {
                cout << "Show AAR, AAR-STD, CAAR and CAAR-STD for one group" << endl << endl;
                cout << "Please enter the group selection (print '0' to get back): " << endl
                    << "1. Beat group" << endl
                    << "2. Meet group" << endl
                    << "3. Miss group" << endl
                    << endl;
                    
                cout << "Please Enter Number 1-3: ";
                cin >> sortGroup;
                getchar();
                cout << endl;
                
                if (sortGroup.at(0) == '0') break;

                while ((sortGroup.at(0) > '3' || sortGroup.at(0) <= '0')) {
                    cout << "Input a Number from 1 to 5 only" << endl << endl;
                    cout << "Please enter the group selection: " << endl
                        << "1. Beat group" << endl
                        << "2. Meet group" << endl
                        << "3. Miss group" << endl
                        << endl;
                    cout << "Please Enter Number 1-3: ";
                    cin >> sortGroup;
                    getchar();
                    cout << endl;
                }
                
                bootstrapResult = BootstrapResult(returnBeat, returnMiss, returnMeet, numDays, sampleSize, etfReturn);
                if (sortGroup.at(0) == '1') displayGroupResult(bootstrapResult[0]);
                else if (sortGroup.at(0) == '2') displayGroupResult(bootstrapResult[1]);
                else displayGroupResult(bootstrapResult[2]);

                // Bootstrap
                
                break; 
            }
                
            case 4: {
                cout << "Show the gnuplot graph with CAAR for all 3 groups" << endl << endl;
                Vector beat_ACAAR, meet_ACAAR, miss_ACAAR;
                beat_ACAAR = bootstrapResult[0][1];
                meet_ACAAR = bootstrapResult[2][1];
                miss_ACAAR = bootstrapResult[1][1];
                visualization CAAR_plot(beat_ACAAR, meet_ACAAR, miss_ACAAR);
                CAAR_plot.Plot();
                
                
                break; 
            }
                
            case 5: {
                running = false;
                cout << "Program Exited" << endl << endl;
                
                break;
            }
        }
    }

    return 0;
}
