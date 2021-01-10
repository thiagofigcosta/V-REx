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

using namespace std;

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


    private:
        // variables
        static const string RESOURCES_FOLDER;
        static const string FILE_SEPARATOR;
};