#include "Syntax.h"

namespace Rosie
{

	Syntax::Syntax()
	{	
		addOpcode("SET", [&](std::vector<int>& args){variables.insert(std::pair<int, Variable>(args[0], variables[args[1]]));});
		addOpcode("ARG", [&](std::vector<int>& args){callStack.push(args[0]);});
		addOpcode("PRINT", [&](std::vector<int>& args){std::cout << variables[args[0]] << std::endl;});
		//addOpcode("CALL", [&](std::vector<int>& args){execute(args[0]);});
		addOpcode("ADD", [&](std::vector<int>& args){variables[args[0]].set(variables[args[1]].get<float>()+variables[args[2]].get<float>());});
		addOpcode("NEG", [&](std::vector<int>& args){variables[args[0]].set(-variables[args[1]].get<float>());});
		addOpcode("SUB", [&](std::vector<int>& args){variables[args[0]].set(variables[args[1]].get<float>()-variables[args[2]].get<float>());});
		addOpcode("MULT", [&](std::vector<int>& args){});
		addOpcode("DIV", [&](std::vector<int>& args){});
		
		addMethod("print", [&](std::vector<Variable>& args){std::cout << args[0] << std::endl;});
	}
	
	int Syntax::getOpcodeId(const std::string& name) const
	{
		return opcodes.at(name).getId();
	}
	
	void Syntax::addOpcode(const std::string& name, const std::function<void(std::vector<int>&)> func)
	{
		opcodes.insert(std::pair<std::string, Function<int>>(name, Function<int>(name, func, opcodes.size())));
	}
	
	void Syntax::addMethod(const std::string& name, const std::function<void(std::vector<Variable>&)> func)
	{
		methods.insert(std::pair<std::string, Function<Variable>>(name, Function<Variable>(name, func, methods.size())));
	}
	
	bool Syntax::hasMethod(const std::string& name) const
	{
		return methods.find(name) != methods.end();
	}
	
	void Syntax::execute(const std::string& name, std::vector<Variable>& arguments) const
	{
		if(methods.find(name) != methods.end())
		{
			methods.at(name).execute(arguments);
		}
	}
	
	void Syntax::execute(const int& id)
	{
		/*for(const auto& pair : methods)
		{
			if(pair.second.getId() == id)
			{
				std::vector<Variable> args;
				while(!callStack.empty())
				{
					args.push_back(variables[callStack.top()]);
					callStack.pop();
				}
				pair.second.execute(args);
			}
		}*/
	}
	
	std::unordered_map<std::string, Function<Variable>> Syntax::getNativeMethods() const
	{
		return methods;
	}

}
