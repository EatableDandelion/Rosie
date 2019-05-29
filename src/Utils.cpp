#include "Utils.h"

namespace Rosie
{
	std::size_t getId(const std::string& name)
	{
		return std::hash<std::string>{}(name);
	}
	
	std::vector<std::string> split(const std::string& line, const std::string& separator)
	{
		std::vector<std::string> res;
		std::size_t start = 0;
		std::size_t end = line.find(separator, start);
		while(end != std::string::npos){
			res.push_back(line.substr(start, end));
			start = end+1;
			end = line.find(separator, start);
		}
		return res;
	}
}
