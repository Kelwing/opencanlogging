#include "include/config.hpp"
#include "include/easylogging++.h"
#include <jsoncpp>

using namespace std;

Config::Config(string filename){
    parse(filename);    
}

void Config::parse(string filename){
    Json::Value root;
    Json::Reader reader;
    ifstream configFile(filename);
    if(!configFile.is_open()){
        LOG(ERROR) << "Could not open config file!";
        return;
    }
    if(!reader.parse(configFile, root, false)){
        LOG(ERROR) << "Could not parse config file: " << getFormattedErrorMessages();
        return;
    }

    if(root.isMember("pids")){
        if(root["pids"].isArray()){
            for(int i = 0; i < root["pids"].size(); i++){
                pids.push_back(root["pids"][i]);
            }
        }
    } else LOG(ERROR) << "pids missing from config!";
}
