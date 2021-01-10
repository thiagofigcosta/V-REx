#include "Utils.hpp"


#ifdef _WIN32 // windows
  const string Utils::FILE_SEPARATOR="\\";
#elif __APPLE__ // mac 
  const string Utils::FILE_SEPARATOR="/";
#else // linux <3
  const string Utils::FILE_SEPARATOR="/";
#endif
const string Utils::RESOURCES_FOLDER="../../res";


Utils::Utils() {
}

Utils::Utils(const Utils& orig) {
}

Utils::~Utils() {
}


vector<string> Utils::splitString(string str, string token){
    vector<string> result;
    while(str.size()){
        size_t index = str.find(token);
        if(index!=string::npos){
            result.push_back(str.substr(0,index));
            str = str.substr(index+token.size());
            if(str.size()==0)result.push_back(str);
        }else{
            result.push_back(str);
            str = "";
        }
    }
    return result;
}

vector<pair<string, vector<float>>> Utils::readLabeledCsvDataset(const string filename){
    vector<pair<string, vector<float>>> result;
    ifstream file(filename);
    if(!file.is_open()) throw runtime_error("Could not open file");
    string line;
    while (getline(file, line)){
        if (!line.empty()){
            pair<string, vector<float>> parsed;
            vector<string> fields = Utils::splitString(line,",");
            parsed.first=fields.back();
            parsed.second={};
            for (vector<string>::size_type i=0;i<fields.size()-1; i++){
                parsed.second.push_back(stof(fields[i]));
            }
            result.push_back(parsed);
        }
    }
    file.close();
    return result;
}

void Utils::ltrimInPlace(string &s) {
    s.erase(s.begin(), find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !isspace(ch);
    }));
}

void Utils::rtrimInPlace(string &s) {
    s.erase(find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !isspace(ch);
    }).base(), s.end());
}

void Utils::trimInPlace(string &s) {
    ltrimInPlace(s);
    rtrimInPlace(s);
}

string Utils::ltrim(string s) {
    ltrimInPlace(s);
    return s;
}

string Utils::rtrim(string s) {
    rtrimInPlace(s);
    return s;
}

string Utils::trim(string s) {
    trimInPlace(s);
    return s;
}

string Utils::getResourcesFolder(){
    return Utils::RESOURCES_FOLDER;
}

string Utils::getResourcePath(string filename){
    return Utils::joinPath(Utils::RESOURCES_FOLDER,filename);
}

string Utils::joinPath(string base,string sufix){
    trimInPlace(base);
    trimInPlace(sufix);
    if (!Utils::endsWith(base,Utils::FILE_SEPARATOR)){
        base+=Utils::FILE_SEPARATOR;
    }
    return base+sufix;
}

bool Utils::endsWith(const string &str, const string &ending){
    if (ending.size() > str.size()) return false;
    return equal(ending.rbegin(), ending.rend(), str.rbegin());
}

bool Utils::runningOnDockerContainer(){
    string path = "/proc/self/cgroup";
    return Utils::checkIfFileExists("/.dockerenv") ||
        (Utils::checkIfIsFile(path) && Utils::checkIfFileContainsString(path,"docker"));
}

bool Utils::checkIfFileExists(const string &path){
    struct stat buffer;   
    return (stat (path.c_str(), &buffer) == 0); 
}

bool Utils::checkIfIsFile(const string &path){
    struct stat buffer;
    if (stat (path.c_str(), &buffer) == 0){
        if (buffer.st_mode & S_IFDIR){
            return false;
        }else if (buffer.st_mode & S_IFREG){
            return true;
        }else{
            throw runtime_error("File "+path+" is a special file!");
        }
    }else{
        throw runtime_error("File "+path+" does not exists!");
    }
}

bool Utils::checkIfFileContainsString(const string &path,const string &str){
    bool found=false;
    ifstream file(path);
    if(!file.is_open()) throw runtime_error("Could not open file");
    string line;
    while(getline(file, line) && !found){
        if(line.find(str) != string::npos){
            found = true;
            break;
        }
    }
    file.close();
    return found;
}