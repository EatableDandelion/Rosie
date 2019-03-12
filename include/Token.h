#pragma once

#include <string>
#include <set>
#include <vector>
#include "Objects.h"

namespace Rosie
{	
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
