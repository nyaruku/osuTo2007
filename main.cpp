#include <iostream>
#include <filesystem>
#include <vector>
#include <fstream>
#include <sstream>
#include <utility>
#include <string.h>

void downgradeOsuFile(std::filesystem::path filePath);
bool is_number(const std::string& s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}
bool lineStartsWith(const char* line, const char* prefix) {
	return strncmp(line, prefix, strlen(prefix)) == 0;
}
std::string replaceString(std::string subject, const std::string& search, const std::string& replace) {
	size_t pos = subject.find(search);
	while (pos != std::string::npos) {
		subject.replace(pos, search.length(), replace);
		pos = subject.find(search, pos + replace.length());
	}
	return subject;
}
bool string_contains(std::string input, std::string key) {
	if (input.find(key) != std::string::npos) {
		return true;
	}
	return false;
}

int main() {
    int option_range = 0;
    std::cout << "##################\nosuTo2007\nosu! : _Railgun_\nDiscord : @railgun_osu\n##################\n\n"; 
    std::vector<std::filesystem::path> map_list;
    for (const auto& entry : std::filesystem::directory_iterator(std::filesystem::current_path())) {
        if (entry.path().extension() == ".osu") {
            map_list.push_back(entry.path()); 
        }
    }
    if(map_list.size() == 0){
         std::cout << "No .osu files found\nExiting...";
         return 0;
    }else{
         std::cout << "Choose which .osu file you want to convert to v3:\n\n";
         if(map_list.size() > 1){
            std::cout << "0 = All listed below\n";
         }
         for(size_t i = 1; i-1 < map_list.size(); i++){
            std::cout << i << " = " << map_list[i-1].filename() << "\n";
            option_range++;
         }

    }
    opt:
    std::cout << "Choose an option:\n";
    std::string input;
    std::cin >> input;
    if(!is_number(input)){ std::cout << "Not a number\n";goto opt;}
    if(!(std::stoi(input)<option_range+1)){ std::cout << "Invalid Option\n";goto opt;}
    if(std::stoi(input)==0){

    }else{
        downgradeOsuFile(map_list[std::stoi(input)-1]);
    }
	return 0;
}

void downgradeOsuFile(std::filesystem::path filePath){
    std::string fileFormat = "";
    std::vector<std::pair <std::string,std::string>> general,metadata,difficulty;
    std::vector<std::string> events,timingPoints,hitObjects;
    int section = 0;
    std::cout << "Downgrading " << filePath.filename() << " to v3 file format...\n";
    std::ifstream file(filePath);
        if (file.is_open())    {
        std::string line;
      
      
    while (std::getline(file, line)) {

        switch(section){
            // case 0: not implemented to skip content immediately
            case 1:
             // General
            break;
            case 2:
             // Metadata
             break;
               case 3:
               // Difficulty
             break;
               case 4:
               // Events
             break;
               case 5:
               // Timing Points
             break;
               case 6:
               // HitObjects
             break;
        }


        if(lineStartsWith(line.c_str(),"osu file format v")){
        fileFormat = replaceString(line,"osu file format v","");
        std::cout << "Target file is version: " << fileFormat << "\n";
        }
         if(lineStartsWith(line.c_str(),"[")){
         // this is fine as it only gets executed once on a new section, code looks shit but it works
       if(lineStartsWith(line.c_str(),"[General]")){section=1;}
       if(lineStartsWith(line.c_str(),"[Editor]")){section=0;} // skip -> not present in 2007 client
       if(lineStartsWith(line.c_str(),"[Metadata]")){section=2;}
       if(lineStartsWith(line.c_str(),"[Difficulty]")){section=3;}
       if(lineStartsWith(line.c_str(),"[Events]")){section=4;}
       if(lineStartsWith(line.c_str(),"[TimingPoints]")){section=5;}
       if(lineStartsWith(line.c_str(),"[Colours]")){section=0;} // skip -> not present in 2007 client
       if(lineStartsWith(line.c_str(),"[HitObjects]")){section=6;}}
    }
    file.close();
    }
}