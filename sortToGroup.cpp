#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include "Group.h"
#include <cmath>
#include <iomanip>
#include "Stock.h"
#include "sortToGroup.h"
using namespace std;

namespace fre{
    void openInputFile(ifstream& inFile){
        const string ANNOUNCEMENT = "Russell3000EarningsAnnouncements.csv";
    
        inFile.open(ANNOUNCEMENT, ios::in);
        while (!inFile){
            cout << "FILE FAILED TO OPEN!" << endl;
            inFile.clear();
            inFile.open(ANNOUNCEMENT, ios::in);
        }
    }


    void processAnnouncement(map<string, vector<string>>& announcementInfoMap, vector<double>& surprisePercent){
        string line;
        string ticker, date, periodEnding, estimate, reported, surprise, surprisePct;
        
        ifstream inFile;
        openInputFile(inFile);
        getline(inFile, line);
        while (!inFile.eof()){
            
            vector<string> attributes;
            getline(inFile, line);
            stringstream sin(line);
            getline(sin, ticker, ',');
            attributes.push_back(ticker);
            getline(sin, date, ',');
            attributes.push_back(date);
            getline(sin, periodEnding, ',');
            attributes.push_back(periodEnding);
            getline(sin, estimate, ',');
            attributes.push_back(estimate);
            getline(sin, reported, ',');
            attributes.push_back(reported);
            getline(sin, surprise, ',');
            attributes.push_back(surprise);
            getline(sin, surprisePct);
            attributes.push_back(surprisePct);
            surprisePercent.push_back(stod(surprisePct));
            announcementInfoMap[ticker] = attributes;
        }
    }
    
    string convertDateFormat(const string& inputDate) {
        stringstream ss(inputDate);
        size_t firstDash = inputDate.find('-');
        size_t secondDash = inputDate.find('-', firstDash + 1);
    
        int day = stoi(inputDate.substr(0, firstDash));
        string month = inputDate.substr(firstDash + 1, secondDash - firstDash - 1);
        int year = std::stoi(inputDate.substr(secondDash + 1));

        static map<string, int> monthMap = {
            {"Jan", 1}, {"Feb", 2}, {"Mar", 3},
            {"Apr", 4}, {"May", 5}, {"Jun", 6},
            {"Jul", 7}, {"Aug", 8}, {"Sep", 9},
            {"Oct", 10}, {"Nov", 11}, {"Dec", 12}
        };

        int monthNumber = monthMap[month];

        ostringstream oss;
        oss << std::setfill('0') << std::setw(4) << year << '-'
        << std::setw(2) << monthNumber << '-'
        << std::setw(2) << day;

        return oss.str();
    }
    
    
    vector<Group> getGroup(){

        vector<Group> groupResult;
        map<string, vector<string>> announcementInfoMap;
        vector<double> surprisePercent; 
        processAnnouncement(announcementInfoMap, surprisePercent);
        sort(surprisePercent.begin(), surprisePercent.end());
        
        int interval = ceil(surprisePercent.size() / 3); //updated by Rita

        double threshold1 = surprisePercent[interval]; 
        double threshold2 = surprisePercent[interval * 2];
        
        Group beat;
        Group miss;
        Group meet;
        
        for (map<string, vector<string>>::iterator itr = announcementInfoMap.begin(); itr != announcementInfoMap.end(); itr++ ){
            Stock s(itr->second[0], convertDateFormat(itr->second[1]), itr->second[2],stod(itr->second[3]),stod(itr->second[4]),
                    stod(itr->second[5]),stod(itr->second[6]));
            if (stod(itr->second[6]) <= threshold1) miss.addStock(s);//miss.push_back(itr->first);
            else if (stod(itr->second[6])  >= threshold2) beat.addStock(s);//beat.push_back(itr->first);
            else meet.addStock(s); //meet.push_back(itr->first);
        }
        
        groupResult.push_back(beat);
        groupResult.push_back(miss);
        groupResult.push_back(meet);
        
        ofstream fout;
        
        
        fout.open("Beat.txt");
        beat.display(fout);
        fout.close();
        
        fout.open("Miss.txt");
        miss.display(fout);
        fout.close();
        
        fout.open("Meet.txt");
        meet.display(fout);
        fout.close();
        
        return groupResult;
        
    }
}
