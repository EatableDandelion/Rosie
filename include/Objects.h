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
			Variable();

			Variable operator+(const Variable& other);
		
			/*int type;

			union
			{
				float f;
				int i;
				bool b;
			};
			*/


		private:
			std::size_t type;
			std::variant<float, int, bool, std::string> value;
		
			Variable performOperation(const Variable& other, const char c);
			
			template<typename T>
			Variable performOperationOnType(const Variable& other, const char c)
			{
				if(c == '+')
				{
					return Variable(std::get<T>(value)+std::get<T>(other.value));			
				}
				else if(c == '-')
				{
					return Variable(std::get<T>(value)-std::get<T>(other.value));			
				}
				else if(c == '*')
				{
					return Variable(std::get<T>(value)*std::get<T>(other.value));			
				}
				else if(c == '/')
				{
					return Variable(std::get<T>(value)/std::get<T>(other.value));			
				}
				else
				{
					error
				}
			}
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
