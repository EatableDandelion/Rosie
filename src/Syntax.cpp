#include "Syntax.h"

namespace Rosie
{	
	bool Syntax::isFunction(const Token& token, const Token& nextToken)
	{
		return (token.type == TokenType::VARNAME) && isListStart(nextToken);
	}
	
	bool Syntax::isFunctionDeclaration(const Token& token)
	{
		return token == "function";
	}
	
	bool Syntax::isAssignment(const Token& token)
	{
		return token == "=";
	}
	
	bool Syntax::isClassDeclaration(const Token& token)
	{
		return token == "define";
	}
	
	bool Syntax::isStartScope(const Token& token)
	{
		return token == "{";
	}
	
	bool Syntax::isEndScope(const Token& token)
	{
		return token == "}";
	}
	
	bool Syntax::isTerminator(const Token& token)
	{
	return token == ";";
	}
	
	bool Syntax::isListSeparator(const Token& token)
	{
		return token == ",";
	}
	
	bool Syntax::isListStart(const Token& token)
	{
		return token == "(";
	}
	
	bool Syntax::isListEnd(const Token& token)
	{
		return token == ")";
	}
	
	void Syntax::setNativeFunctions(const std::vector<std::string>& nativeFunc)
	{
		nativeFunctions = nativeFunc;
	}
	
	std::vector<std::string> Syntax::getNativeFunctions() const
	{
		return nativeFunctions;
	}
}
