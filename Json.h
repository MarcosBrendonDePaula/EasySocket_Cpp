#pragma once
#include <map>
#include <cstring>
#include <sstream>
#include <vector>
#include <iostream>
class Json{
	public:
		std::vector<std::string> split(const std::string &s, char delim);
		std::map<std::string,std::string> variaveis;
		std::map<std::string,Json> SubJsons; 
		void addVar(std::string name,std::string valor);
		void addVarPath(std::string path,std::string name,std::string valor);
		void addList(std::string name,std::vector<std::string> list);
		void addListPath(std::string path,std::string name,std::vector<std::string> list);
		std::string getValPath(std::string path,std::string name);
		std::string getVal(std::string path);
		std::string to_string();
	private:
		std::string GenScopo(Json atual);
};