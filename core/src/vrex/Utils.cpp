#include "Utils.hpp"


#ifdef _WIN32 // windows
  const string Utils::FILE_SEPARATOR="\\";
#elif __APPLE__ // mac 
  const string Utils::FILE_SEPARATOR="/";
#else // linux <3
  const string Utils::FILE_SEPARATOR="/";
#endif
const string Utils::RESOURCES_FOLDER="../../res";
default_random_engine Utils::RNG = default_random_engine{};

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
    if(!file.is_open()) {
        throw runtime_error("Could not open file");
    }
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

vector<pair<vector<int>, vector<float>>> Utils::extractSubVector(const vector<pair<vector<int>, vector<float>>> &vec, int start, int size){
    if ((size_t)start>=vec.size()){
        throw runtime_error("Start position ("+to_string(start)+") bigger than vector size ("+to_string(vec.size())+")");
    }
    int diff=vec.size()-(start+size);
    if (diff<0){
        size+=diff;
    }
    return {vec.begin()+start, vec.begin()+start+size };
}

pair<vector<pair<int, vector<float>>>,map<string,int>> Utils::enumfyDataset(const vector<pair<string, vector<float>>> &vec){
    set<string> label;
    for (pair<string, vector<float>> entry:vec){
        label.insert(entry.first);
    }
    map<string,int> equivalence;
    int i=0;
    for (string l:label){
        equivalence[l]=i++;
    }
    vector<pair<int, vector<float>>> enumfied_data;
    for (pair<string, vector<float>> entry:vec){
        enumfied_data.push_back(pair<int, vector<float>>(equivalence[entry.first],entry.second));
    } 
    return pair<vector<pair<int, vector<float>>>,map<string,int>> (enumfied_data,equivalence);
}

vector<pair<vector<int>, vector<float>>> Utils::shuffleDataset(const vector<pair<vector<int>, vector<float>>> &vec){
    vector<int> indexes(vec.size());
    iota (indexes.begin(), indexes.end(), 0);
    shuffle(indexes.begin(), indexes.end(), Utils::RNG);
    vector<pair<vector<int>, vector<float>>> out;
    for(int i:indexes){
        out.push_back(vec[i]);
    }
    return out;
}

pair<vector<pair<vector<int>, vector<float>>>,vector<pair<vector<int>, vector<float>>>> Utils::divideDataSet(const vector<pair<vector<int>, vector<float>>> &vec, float percentageOfFirst){
    size_t fristSize=vec.size()*percentageOfFirst;
    return pair<vector<pair<vector<int>, vector<float>>>,vector<pair<vector<int>, vector<float>>>> (Utils::extractSubVector(vec, 0, fristSize),Utils::extractSubVector(vec, fristSize, vec.size()-fristSize));
}

vector<pair<vector<int>, vector<float>>> Utils::encodeDatasetLabels(const vector<pair<int, vector<float>>> &vec, DataEncoder enc){
    int max=numeric_limits<int>::min();
    set<int> values;
    for (pair<int, vector<float>> entry:vec){
        values.insert(entry.first);
        if (entry.first > max){
            max = entry.first;
        }
    }

    int output_neurons=0;

    switch(enc){
        case DataEncoder::BINARY:
            output_neurons=ceil(log2(max+1));
            break;
        case DataEncoder::SPARSE:
            output_neurons=max+1;
            break;
        case DataEncoder::INCREMENTAL:
        case DataEncoder::INCREMENTAL_PLUS_ONE:
        case DataEncoder::EXPONENTIAL:
            output_neurons=1;
            break;
    }

    map<int,vector<int>> equivalence;
    for (int l:values){
        vector<int> array;
        for (int i = 0; i < output_neurons; ++i) {
            switch(enc){
                case DataEncoder::BINARY:
                    array.push_back((l >> i) & 1);
                    break;
                case DataEncoder::SPARSE:
                    array.push_back((int) (i==l));
                    break;
                case DataEncoder::INCREMENTAL:
                    array.push_back(l);
                    break;
                case DataEncoder::INCREMENTAL_PLUS_ONE:
                    array.push_back(l+1);
                    break;
                case DataEncoder::EXPONENTIAL:
                    array.push_back(pow(2,l+1));
                    break;
            }
        }
        equivalence[l]=array;
    }
    vector<pair<vector<int>, vector<float>>> encoded_data;
    for (pair<int, vector<float>> entry:vec){
        encoded_data.push_back(pair<vector<int>, vector<float>>(equivalence[entry.first],entry.second));
    }
    return encoded_data;
}
