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
		res.push_back(line.substr(start, end));
		
		return res;
	}
	
	ErrorDisplay::ErrorDisplay()
	{
		start();
	}
	
	ErrorDisplay::~ErrorDisplay()
	{
		end();
	}
	
	void ErrorDisplay::start() const
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
		std::cout << std::endl;
	}
			
	void ErrorDisplay::show(const std::string& msg, const bool& warningOn) const
	{
		if(warningOn)
		{
			std::cout << "/!\\ ";
		}
		
		std::cout << msg;
		
		if(warningOn)
		{
			std::cout << " /!\\";
		}
		std::cout << std::endl;	
	}
	
	void ErrorDisplay::end() const
	{
		std::cout << std::endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		exit(EXIT_FAILURE);
	}
	
	
	BaseError::BaseError(const std::string& msg):msg(msg)
	{}
			
	const char*  BaseError::what() const throw()
	{
		ErrorDisplay().show(msg);
		return "";
	}
			
}
