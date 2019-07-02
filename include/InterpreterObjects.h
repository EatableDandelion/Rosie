#pragma once

#include <memory>
#include <vector>
#include <string>
#include <iostream>
#include <unordered_map>
#include <map>
#include <algorithm>
#include <stack>
#include "Utils.h"

namespace Rosie{
	
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
	/*10*/	CSTARRAY,
	/*11*/	FUNCNAME,	//any function
	/*12*/	UNDEFINED	//base value
	};
	
	enum Category
	{	
		CONSTANT,
		VARIABLE,
		FUNCTION,
		INTEGER
	};
		
	struct Address
	{
		public:
			Address(const int& id, const Category& category = Category::CONSTANT, const std::string& name = "", const TokenType& type = TokenType::UNDEFINED);
			Address(const Address& address);
			Address();
			
			int getId() const;
			std::string getName() const;
			Category getCategory() const;
			std::string getString() const;
			int getTypeId() const;
			void setType(const TokenType& tokenType);
			TokenType getType() const;
			void setScope(const int& scope);
			int getScope();
		
		private:
			int id;
			std::string name;
			Category category;
			TokenType type;
			int scope;
	};
	
	struct AddressMap //collection of addresses
	{
		public:	
			AddressMap(const Category& category, const int& startIndex = 0);
		
			Address newAddress(const std::string& name);
			
			Address newAddress(const std::string& name, const TokenType& tokenType);
			
			Address getAddress(const std::string& name);
			
			bool hasAddress(const std::string& name);
		
			std::vector<Address> getAddresses() const;
			
			void startScope();
			
			void endScope();
						
		private:
			std::unordered_map<std::size_t, Address> addresses;
			int head;
			std::stack<int> scope;
			Category category;
	};
	
	struct Constant
	{
		public:
			Constant(const TokenType& type, const std::string& value);
			int getTypeId() const;
			std::string getValue() const;
		
		private:
			TokenType type;
			std::string value;
	};

}
