#pragma once

#include <memory>
#include <vector>
#include <string>
#include <iostream>
#include <variant>

namespace Rosie{
	
	enum AddressType
	{
		INTEGER,
		VARIABLE,
		CONSTANT,
		FUNCTION,
	};
	
	struct Address
	{
		Address(const std::size_t& location, const std::string& name = "", const std::size_t& type = 0);
		Address(const Address& address);
		Address();
		std::size_t id;
		std::string name;
		std::size_t type;
	};
	
	struct Variable
	{
		public:
			Variable(const float& floatValue);
			Variable(const int& integerValue);
			Variable(const bool& booleanValue);
			Variable(const std::string& stringValue);
			Variable();

			
			template<typename T>
			T get() const
			{
				return std::get<T>(value);
			}
			
			void set(const float& newValue);
			void set(const int& newValue);
			void set(const bool& newValue);
			void set(const std::string& newValue);
			
			friend std::ostream& operator <<(std::ostream& os, const Variable& var)
			{
				if(var.type == 0)
				{
					os << std::to_string(var.get<float>())+ " (float)"; 
				}
				else if(var.type == 1)
				{
					os << std::to_string(var.get<int>())+ " (int)"; 
				}
				else if(var.type == 2)
				{
					os << std::to_string(var.get<bool>())+ " (boolean)"; 
				}
				else if(var.type == 3)
				{
					os << var.get<std::string>()+ " (string)"; 
				}
				else
				{
					os << "Error: undefined variable type: type id = "+std::to_string(var.type);
				}
				return os;
			}
			
		private:
			std::size_t type;
			std::variant<float, int, bool, std::string> value;
	};
		
	enum TokenType
	{
	/*0*/	KEYWORD,	//if, while, return
	/*1*/	SEPARATOR, 	//( ) ;
	/*2*/	OPERATOR,	//+ =
	/*3*/	COMPARATOR, //>, <, ==, &&
	/*4*/	VARNAME, 	//all the variables
	/*5*/	VARTYPE,	//float, int, double, string, var
	/*6*/	CSTSTRING,	//"a"
	/*7*/	CSTFLOAT,	//58.0
	/*8*/	CSTINT,		//5
	/*9*/	CSTBOOLEAN,	//true, false
	/*10*/	FUNCNAME,	//any function
	/*11*/	UNDEFINED	//base value
	};
	
	const std::vector<std::string> typeNames {	"Keyword", 
												"Separator", 
												"Operator", 
												"Comparator", 
												"Variable name", 
												"Variable type",
												"String constant",
												"Float constant",
												"Integer constant",
												"Boolean constant",
												"Function name",
												"Undefined"};
	
	struct Token
	{
		Rosie::TokenType type = UNDEFINED;
		std::string value;
		
		void operator+=(const char c);
		
		bool operator==(const std::string& stringValue);
		
		bool operator!=(const std::string& stringValue);
		
		int length() const;
		
		void clear();
		
		friend std::ostream& operator <<(std::ostream& os, const Token& token)
		{
			os << token.value+" ("+Rosie::typeNames[token.type]+")"  ;
			return os;
		}
		
		void setAddress(const Address& address);
		
		Address getAddress() const;
		
		private:
			Address m_address;
	};

}
