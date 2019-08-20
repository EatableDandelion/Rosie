#pragma once

#include <vector>
#include <iostream>
#include <functional>
#include "Token.h"


namespace Rosie
{	
	class Syntax
	{
		public:
			bool isFunction(const Token& token, const Token& nextToken);
			bool isFunctionDeclaration(const Token& token);	
			bool isAssignment(const Token& token);
			bool isClassDeclaration(const Token& token);
			bool isStartScope(const Token& token);	
			bool isEndScope(const Token& token);
			bool isTerminator(const Token& token);
			bool isSeparator(const Token& token);
			bool isArgStart(const Token& token);	
			bool isArgEnd(const Token& token);
			bool isArrayStart(const Token& token);	
			bool isArrayEnd(const Token& token);
			
			void setNativeFunctions(const std::vector<std::string>& nativeFunctions);
			std::vector<std::string> getNativeFunctions() const;
			
		private:
			std::vector<std::string> nativeFunctions;
	};
	
	
}
