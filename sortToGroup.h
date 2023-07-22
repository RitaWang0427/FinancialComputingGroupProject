#pragma once
#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>
#include <sstream>
#include "Group.h"
#include <algorithm>
using namespace std;

namespace fre{
    void openInputFile(ifstream& inFile);
    void processAnnouncement(map<string, vector<string>>& announcementInfoMap, vector<double>& surprisePercent);
    string convertDateFormat(const string& inputDate);
    vector<Group> getGroup();
    
}
