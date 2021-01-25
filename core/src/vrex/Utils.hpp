#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <utility> // std::pair
#include <stdexcept> // std::runtime_error
#include <sstream> // std::stringstream
#include <algorithm> 
#include <cctype>
#include <locale>
#include <sys/stat.h>
#include <stdexcept>
#include <set>
#include <random>
#include <map>
#include <limits>
#include <cmath>
#include <memory>
#include <boost/uuid/uuid.hpp>            
#include <boost/uuid/uuid_generators.hpp> 
#include <boost/uuid/uuid_io.hpp>         

using namespace std;

enum class DataEncoder { BINARY, SPARSE, INCREMENTAL, INCREMENTAL_PLUS_ONE, EXPONENTIAL, DISTINCT_SPARSE, BINARY_PLUS_ONE };

template <typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args){ // this is from C++14
  return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

typedef std::basic_string<char> string;

class Utils{
    public:
        // constructors and destructor
        Utils();
        Utils(const Utils& orig);
        virtual ~Utils();

        // methods
        static vector<string> splitString(string str, string token);
        static vector<pair<string, vector<float>>> readLabeledCsvDataset(const string filename);
        static string getResourcesFolder();
        static string getResourcePath(string filename);
        static string joinPath(string base,string sufix);
        static void ltrimInPlace(string &s);
        static void rtrimInPlace(string &s);
        static void trimInPlace(string &s);
        static string ltrim(string s);
        static string rtrim(string s);
        static string trim(string s);
        static bool endsWith(const string &str, const string &ending);
        static bool runningOnDockerContainer();
        static bool checkIfFileExists(const string &path);
        static bool checkIfIsFile(const string &path);
        static bool checkIfFileContainsString(const string &path,const string &str);
        static pair<vector<pair<int, vector<float>>>,map<string,int>> enumfyDataset(const vector<pair<string, vector<float>>> &vec);
        static vector<pair<vector<int>, vector<float>>> extractSubVector(const vector<pair<vector<int>, vector<float>>> &vec, int start, int size);
        static vector<pair<vector<int>, vector<float>>> encodeDatasetLabels(const vector<pair<int, vector<float>>> &vec, DataEncoder enc);
        static vector<pair<vector<int>, vector<float>>> shuffleDataset(const vector<pair<vector<int>, vector<float>>> &vec);
        static pair<vector<pair<vector<int>, vector<float>>>,vector<pair<vector<int>, vector<float>>>> divideDataSet(const vector<pair<vector<int>, vector<float>>> &vec, float percentageOfFirst);
        static pair<vector<pair<float,float>>,vector<pair<vector<int>, vector<float>>>> normalizeDataset(const vector<pair<vector<int>, vector<float>>> &vec);
        static float getRandomBetweenZeroAndOne();
        static boost::uuids::uuid genRandomUUID();
        static string genRandomUUIDStr();

        // variables
        static mt19937_64 RNG;

    private:
        // methods
        static mt19937_64 getRandomGenerator();
        static uniform_real_distribution<float> dist_zero_one;
        // variables
        static string RESOURCES_FOLDER;
        static const string FILE_SEPARATOR;
};