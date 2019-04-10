#pragma once

#include <memory>
#include <vector>
#include <string>
#include <iostream>
#include <unordered_map>
#include <map>
#include "Utils.h"

namespace Rosie{
	
	struct Member
	{
		Member(const std::size_t typeId, const int& offset);
		std::size_t typeId;
		int offset;
	};
	
	struct Type
	{
		public:
			bool operator==(const Type& other);
			Type();
			Type(const Type& other);
			
		private:
			Type(const std::size_t& id);
			std::size_t id;
			std::unordered_map<std::size_t, Member> members;
			
			void addMember(const std::size_t& name, const std::size_t& typeId);
			
			friend struct TypeCollection;
	};
	
	struct TypeCollection
	{
		public:
			TypeCollection();
			void addType(const std::string& name);
			void addMemberToType(const Type& type, const std::string& memberName, const std::string& memberType);
			int getSize(const Type& type);
			Type getType(const std::string& name) const;
			bool hasType(const std::string& name) const;
			
		private:
			std::map<std::size_t, Type> types;
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
			Address(const int& id, const Type& type, const Category& category = Category::CONSTANT, const std::string& name = "");
			Address(const Address& address);
			Address();
			
			int getId() const;
			std::string getName() const;
			Category getCategory() const;
			Type getType() const;
			std::string getString() const;
		
		private:
			int id;
			std::string name;
			Category category;
			Type type;
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
	/*11*/	CONSTRUCTOR,//class constructors
	/*12*/	UNDEFINED	//base value
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
												"Constructor",
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
			os << token.getString();
			return os;
		}
		
		void setAddress(const Address& address);
		
		Address getAddress() const;
		
		std::string getString() const;
		
		private:
			Address m_address;
	};
	
	struct Constant
	{
		Constant(const std::string& name, const std::string& value);
		std::size_t typeId;
		std::string value;
	};

}
