#pragma once

#include <string>
#include <set>
#include <vector>
#include "InterpreterObjects.h"

namespace Rosie
{	
	
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
		
		bool operator==(const std::string& stringValue) const;
		
		bool operator!=(const std::string& stringValue) const;
		
		int length() const;
		
		void clear();
		
		friend std::ostream& operator <<(std::ostream& os, const Token& token)
		{
			os << token.getString();
			return os;
		}
		
		/*void setAddress(const Address& address);
		
		Address getAddress() const;*/
		
		std::string getString() const;
		
		private:
			//Address m_address;
	};
	
	
	class TypeCaster
	{
		public:
			virtual void assign(Token& token) = 0;

	};
	
	class SpecialCharCaster : public TypeCaster//Separator or operator
	{
		public:
			virtual void assign(Token& token);
			
			bool isSpecialChar(const char c) const;
			
		private:
			bool isOperator(const std::string& name) const;
			bool isSeparator(const std::string& name) const;
			bool isComparator(const std::string& name) const;
	};
	
	class LiteralCaster : public TypeCaster//Keyword, name, type or boolean value
	{
		public:
			LiteralCaster();
			
			virtual void assign(Token& token);
			
		private:
			bool isKeyword(const std::string& name);
			bool isType(const std::string& name);
			bool isBoolean(const std::string& name);
			std::set<std::string> types;
	};
	
	class StringCaster : public TypeCaster//Value
	{
		public:
			virtual void assign(Token& token);
	};
	
	class VoidCaster : public TypeCaster //White spaces and comments
	{
		public:
			virtual void assign(Token& token);
	};
}
