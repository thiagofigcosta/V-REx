#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <utility> // std::pair
#include <stdexcept> // std::runtime_error
#include <sstream> // std::stringstream

using namespace std;

vector<string> split(string str, string token){
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

vector<pair<string, vector<float>>> read_csv(string filename){
    vector<pair<string, vector<float>>> result;
    ifstream file(filename);
    if(!file.is_open()) throw runtime_error("Could not open file");

    string line;
    while (getline(file, line)){
        if (!line.empty()){
            pair<string, vector<float>> parsed;
            vector<string> fields = split(line,",");
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

int main() {
    vector<pair<string, vector<float>>> three_cols = read_csv("../../../../res/iris.data");
    
    for (pair<string, vector<float>> row : three_cols) {
        cout << "------------" <<endl;
        cout << row.first <<endl;
        for (float el : row.second ){
            cout << el<< " " ;
        }
        cout <<endl;
        cout << "------------" <<endl;
    }
    
    return 0;
}