#include <fstream>
#include <map>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
using namespace std;

namespace fre{
    string modifyDate(const string& zero, int N);
    void getSubMap(std::map<std::string, double>& original_map, const std::string& base_key, int numDays);
    map<string, double> readValue(Stock s, int numDays);
}
