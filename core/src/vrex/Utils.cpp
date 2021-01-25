#include "Utils.hpp"


#ifdef _WIN32 // windows
  const string Utils::FILE_SEPARATOR="\\";
#elif __APPLE__ // mac 
  const string Utils::FILE_SEPARATOR="/";
#else // linux <3
  const string Utils::FILE_SEPARATOR="/";
#endif
string Utils::RESOURCES_FOLDER="../../res";
mt19937_64 Utils::RNG = Utils::getRandomGenerator();
uniform_real_distribution<float> Utils::dist_zero_one=uniform_real_distribution<float>(0,nextafter(1, numeric_limits<float>::max()));

Utils::Utils() {
}

Utils::Utils(const Utils& orig) {
}

Utils::~Utils() {
}

mt19937_64 Utils::getRandomGenerator(){
    random_device rd;
    mt19937_64 mt(rd());
    return mt;
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
    if(!file.is_open()) throw runtime_error("Could not open file: "+filename);
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
    if (Utils::runningOnDockerContainer()){ 
        Utils::RESOURCES_FOLDER="/vrex/res";
    }
    return Utils::RESOURCES_FOLDER;
}

string Utils::getResourcePath(string filename){
    return Utils::joinPath(Utils::getResourcesFolder(),filename);
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
        case DataEncoder::BINARY_PLUS_ONE:
            output_neurons=ceil(log2(max+1));
            break;
        case DataEncoder::SPARSE:
        case DataEncoder::DISTINCT_SPARSE:
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
                case DataEncoder::DISTINCT_SPARSE:
                    array.push_back((int) (i==l)*(l+1));
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
                case DataEncoder::BINARY_PLUS_ONE:
                    array.push_back(((l+1) >> i) & 1);
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

pair<vector<pair<float,float>>,vector<pair<vector<int>, vector<float>>>> Utils::normalizeDataset(const vector<pair<vector<int>, vector<float>>> &vec){
    vector<pair<vector<int>, vector<float>>> normalized;
    vector<pair<float,float>> scale;
    vector<float> min;
    vector<float> max;
    for (auto& dummy:vec[0].second){
        (void) dummy;
        min.push_back(numeric_limits<float>::max());
        max.push_back(numeric_limits<float>::min());
    }
    for(pair<vector<int>, vector<float>> entry:vec){
        for (size_t i=0;i<entry.second.size();i++){
            if (entry.second[i]<min[i]){
                min[i]=entry.second[i];
            }
            if (entry.second[i]>max[i]){
                max[i]=entry.second[i];
            }
        }
    }
    for (size_t i=0;i<vec[0].second.size();i++){
        scale.push_back(pair<float,float>(min[i],(max[i]-min[i])));
    }
    
    for(pair<vector<int>, vector<float>> entry:vec){
        vector<float> normalized_entry_labels;
        for (size_t i=0;i<entry.second.size();i++){
            normalized_entry_labels.push_back( (entry.second[i]-scale[i].first)/scale[i].second );
        }
        normalized.push_back(pair<vector<int>, vector<float>>(entry.first,normalized_entry_labels));
    }
    return pair<vector<pair<float,float>>,vector<pair<vector<int>, vector<float>>>>(scale,normalized);
}

float Utils::getRandomBetweenZeroAndOne(){
    return dist_zero_one(Utils::RNG);
}

boost::uuids::uuid Utils::genRandomUUID(){
    return boost::uuids::random_generator()();
}

string Utils::genRandomUUIDStr(){
    return boost::uuids::to_string(Utils::genRandomUUID());
}

string Utils::msToHumanReadable(long timestamp){
    int D=int(timestamp/1000/60/60/24);
    int H=int(timestamp/1000/60/60%24);
    int M=int(timestamp/1000/60%60);
    int S=int(timestamp/1000%60);
    int MS=int(timestamp%1000);

    string out="";
    if (timestamp <= 0)
        out="0 ms";
    if (D > 0)
        out+=to_string(D)+" days ";
    if (D > 0 and MS == 0 and S == 0 and M == 0 and H > 0)
        out+="and ";
    if (H > 0)
        out+=to_string(H)+" hours ";
    if ((D > 0 or H > 0) and MS == 0 and S == 0 and M == 0)
        out+="and ";
    if (M > 0)
        out+=to_string(M)+" minutes ";
    if ((D > 0 or H > 0 or M > 0) and MS == 0 and S == 0)
        out+="and ";
    if (S > 0)
        out+=to_string(S)+" seconds ";
    if ((D > 0 or H > 0 or M > 0 or S > 0) and MS == 0)
        out+="and ";
    if (MS > 0)
        out+=to_string(MS)+" milliseconds ";
    return out;
}