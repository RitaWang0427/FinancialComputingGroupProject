#pragma once
#include <iostream>
using namespace std;

namespace fre{
    class Stock{
        private:
        string ticker;
        string zero; //stock's annoucenment date
        string periodEnding;
        double estimate;
        double reported;
        double surprise;
        double surprisePercentage;
        
        public:
        Stock() : ticker(""), zero(""), periodEnding(""), estimate(0.0), reported(0.0), surprise(0.0), surprisePercentage(0.0) {}
        Stock(string ticker, string zero, string periodEnding, double estimate, double reported, double surprise, double surprisePercentage)
            :ticker(ticker), zero(zero), periodEnding(periodEnding), estimate(estimate), reported(reported), surprise(surprise), surprisePercentage(surprisePercentage){};
        ~Stock() {}
        
        string getTicker() const{ return ticker; }
        string getZero() const{ return zero; }
        string getPeriodEnding() const {return periodEnding;}
        double getReported() const {return reported;}
        double getEstimate() const {return estimate;}
        double getSurprisePercentage() const {return surprisePercentage;}
        double getSurprise() const{ return surprise; }
        
        void setTicker(string& tickerName) { ticker = tickerName;}
        void setZero(string& zeroDate) {zero = zeroDate; }
        void setPeriodEnding(string& periodEnd) {periodEnding = periodEnd;}
        void setEstimate(double& est){estimate = est;}
        void setReported(double& report){reported = report;}
        void setSurprise(double& surpriseNumber){surprise = surpriseNumber;}
        void setsurprisePercentage(double& surprisePer){surprisePercentage = surprisePer;}
        void display(){
            cout << "ticker: " << ticker << " annoucement date: " << zero << " surprise: "<< surprise <<endl;
        }
    };
}
