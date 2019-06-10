#include "Syntax.h"

namespace Rosie
{
	void Syntax::addMethod(const std::string& name, const std::function<void(std::vector<Variable>&)> func)
	{
		methods.add(methods.size(), name, Function<Variable>(name, func, methods.size()));
	}
	
	bool Syntax::hasMethod(const std::string& name) const
	{
		return methods.contains(name);
	}
	/*
	void Syntax::execute(const std::string& name, std::vector<Variable>& arguments, State& state) const
	{
		methods[name].execute(arguments, state);
	}
	
	void Syntax::execute(const int& id, State& state)
	{
		std::vector<Variable> args;
		while(!state.empty())
		{
			args.push_back(state.pop());
		}
		methods[id].execute(args, state);
	}*/
	
	Function<Variable> Syntax::getMethod(const int& id) const
	{
		return methods[id];
	}
	
	std::vector<Function<Variable>> Syntax::getNativeMethods() const
	{
		return methods.getValues();
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
}
