#include <stdio.h>
#include <cstring>
#include <string> 
#include <iostream>
#include <sstream>  
#include <vector>
#include <locale>
#include <iomanip>
#include <fstream>
#include <chrono>
#include "curl/curl.h"
#include "Group.h"
#include <cmath>
#include "Stock.h"
#include "readData.h"
#include <ctime>
#include <chrono>
#include <cstdlib>
#include <exception>
using namespace std::chrono;

using namespace std;

namespace fre{
    
    string modifyDate(const string& zero, int N){
    	std::istringstream iss(zero);
    	std::tm tm {};
    	iss >> std::get_time(&tm, "%Y-%m-%d");

    	std::time_t t = std::mktime(&tm);
    	t += N * 24 * 60 * 60; // Add/subtract N days in seconds

    	std::tm* result = std::localtime(&t);

    	std::ostringstream oss;
    	oss << std::put_time(result, "%Y-%m-%d");

    	return oss.str();
    }
    int write_data(void* ptr, int size, int nmemb, FILE* stream){
		size_t written;
		written = fwrite(ptr, size, nmemb, stream);
		return written;
	}
	struct MemoryStruct {
		char* memory;
		size_t size;
	};
	void* myrealloc(void* ptr, size_t size)
	{
		if (ptr)
			return realloc(ptr, size);
		else
			return malloc(size);
	}
	int write_data2(void* ptr, size_t size, size_t nmemb, void* data)
	{
		size_t realsize = size * nmemb;
		struct MemoryStruct* mem = (struct MemoryStruct*)data;
		mem->memory = (char*)myrealloc(mem->memory, mem->size + realsize + 1);
		if (mem->memory) {
			memcpy(&(mem->memory[mem->size]), ptr, realsize);
			mem->size += realsize;
			mem->memory[mem->size] = 0;
		}
		return realsize;
	}
	void getSubMap(map<string, double>& original_map, const string& base_key, int numDays) {
        	auto base_iter = original_map.find(base_key);
        	if (base_iter == original_map.end()) {
                	throw runtime_error("");
        	}
        	if(distance(original_map.begin(), base_iter) <= numDays){
                	throw runtime_error("");
        	}
        	if(distance(base_iter,original_map.end()) <= numDays){
                	throw runtime_error("");
        	}
        	auto start_iter = std::prev(base_iter, numDays);
       
        	auto end_iter = std::next(base_iter, numDays+1);
        	string startDate = start_iter -> first;
        	string endDate = end_iter -> first;
        	map<std::string, double> sub_map(start_iter, end_iter);
        	original_map.swap(sub_map);
        }

    map<string, double> readValue(Stock s, int numDays){
        // declaration of an object CURL
        vector<string> symbolList;
		symbolList.push_back(s.getTicker());
		map<string, double> resultMap;
		//populateSymbolVector(symbolList);

		// file pointer to create file that store the data  
		FILE* fp;
		const char resultfilename[FILENAME_MAX] = "temp.txt";

		// declaration of an object CURL 
		CURL* handle;

		CURLcode result;

		// set up the program environment that libcurl needs 
		curl_global_init(CURL_GLOBAL_ALL);

		// curl_easy_init() returns a CURL easy handle 
		handle = curl_easy_init();

		// if everything's all right with the easy handle... 
		if (handle)
		{
			string url_common = "https://eodhistoricaldata.com/api/eod/";
			string start_date = modifyDate(s.getZero(),-(numDays*1.55));
			string end_date =  modifyDate(s.getZero(),numDays*1.55);
			string api_token = "64469873de4478.25826487";  // You must replace this API token with yours
			
			vector<string>::iterator itr = symbolList.begin();
			for (; itr != symbolList.end(); itr++)
			{
				struct MemoryStruct data;
				data.memory = NULL;
				data.size = 0;

				string symbol = *itr;
				string url_request = url_common + symbol + ".US?" + "from=" + start_date + "&to=" + end_date + "&api_token=" + api_token + "&period=d";
				curl_easy_setopt(handle, CURLOPT_URL, url_request.c_str());
				

				//adding a user agent
				curl_easy_setopt(handle, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:74.0) Gecko/20100101 Firefox/74.0");
				curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, 0);
				curl_easy_setopt(handle, CURLOPT_SSL_VERIFYHOST, 0);
				fp = fopen(resultfilename, "w");
				fprintf(fp, "%s\n", symbol.c_str());
				curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data);
				curl_easy_setopt(handle, CURLOPT_WRITEDATA, fp);
				result = curl_easy_perform(handle);
				

			// check for errors 
				if (result != CURLE_OK) {
					throw std::invalid_argument(string("curl_easy_perform() failed: ") + curl_easy_strerror(result));
					//return resultPair;
				}

				curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data2);
				curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void*)&data);

				// perform, then store the expected code in result
				result = curl_easy_perform(handle);
				fprintf(fp, "%c", '\n');
				
				//fclose(fp);
				if (result != CURLE_OK)
				{
					// if errors have occured, tell us what is wrong with result
					throw std::invalid_argument(string("curl_easy_perform() failed: ") + curl_easy_strerror(result));
					//return resultPair;
				}

				stringstream sData;
				sData.str(data.memory);
				
				string sValue, sDate;
				double dValue = 0;
				string line;
	
				
			//cout << symbol << endl;;
				while (getline(sData, line)) {
					size_t found = line.find('-');
					if (found != std::string::npos)
					{
						
						sDate = line.substr(0, line.find_first_of(','));
						string temp = line;
                		temp.erase(0, line.find_first_of(',') + 1);  // Remove the first column
                		temp.erase(0, temp.find_first_of(',') + 1);  // Remove the second column
                		temp.erase(0, temp.find_first_of(',') + 1);  // Remove the third column
                		temp.erase(0, temp.find_first_of(',') + 1);  // Remove the fourth column
                		temp.erase(0, temp.find_first_of(',') + 1);  // Remove the fifth column
            
                		sValue = temp.substr(0, temp.find_first_of(','));
                		dValue = strtod(sValue.c_str(), NULL);
                		//fprintf(fp, "Date = %s dMaxOpen = %.2f\n", sDate.c_str(), dValue);
                		resultMap[sDate] = dValue;
					}
				}
			
				//fprintf(fp, "Symbol = %s dMaxOpen = %.2f\n", itr->c_str(), max_open);
            	fclose(fp);
			
				free(data.memory);
				data.size = 0;
			}

		}
		else
		{
			//fprintf(stderr, "Curl init failed!\n");
			throw invalid_argument("Curl init failed!\n");
			//return resultPair;
		}
		
		
		// cleanup since you've used curl_easy_init  
		curl_easy_cleanup(handle);
	
		// release resources acquired by curl_global_init() 
		curl_global_cleanup();
		try{
			getSubMap(resultMap, s.getZero(),numDays);
		}catch(const std::exception &e) {
			return {};
		}
		return resultMap;
    }
}
