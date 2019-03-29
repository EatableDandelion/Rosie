#include "Syntax.h"

namespace Rosie
{

	Syntax::Syntax()
	{	
		addOpcode("SET", [&](std::vector<int>& args){
			if(args[0] == 0)
			{
				variables.insert(std::pair<int, Variable>(args[1], constants[args[2]]));
			}
			else if(args[0] == 1)
			{
				variables.insert(std::pair<int, Variable>(args[1], getVariable(args[2])));
			}
			});
		addOpcode("ARG", [&](std::vector<int>& args){callStack.push(args[0]);});
		addOpcode("PRINT", [&](std::vector<int>& args){std::cout << getVariable(args[0]) << std::endl;});
		addOpcode("CALL", [&](std::vector<int>& args){execute(args[0]);});
		addOpcode("ADD", [&](std::vector<int>& args){callStack.push(Variable(getVariable(args[0]).get<float>()+getVariable(args[1]).get<float>());});
		addOpcode("NEG", [&](std::vector<int>& args){callStack.push(Variable(-getVariable(args[0]).get<float>());});
		addOpcode("SUB", [&](std::vector<int>& args){callStack.push(Variable(getVariable(args[0]).get<float>()-getVariable(args[1]).get<float>());});
		addOpcode("MULT", [&](std::vector<int>& args){});
		addOpcode("DIV", [&](std::vector<int>& args){});
		
		addMethod("print", [&](std::vector<Variable>& args){std::cout << args[0] << std::endl;});
	}
	
	int Syntax::getOpcodeId(const std::string& name) const
	{
		if(!opcodes.contains(name)) std::cout << "Opcode "+name+" unrecognized" << std::endl;
		return opcodes[name].getId();
	}
	
	void Syntax::addOpcode(const std::string& name, const std::function<void(std::vector<int>&)> func)
	{
		opcodes.add(opcodes.size(), name, Function<int>(name, func, opcodes.size()));
	}
	
	void Syntax::addMethod(const std::string& name, const std::function<void(std::vector<Variable>&)> func)
	{
		methods.add(methods.size(), name, Function<Variable>(name, func, methods.size()));
	}
	
	bool Syntax::hasMethod(const std::string& name) const
	{
		return methods.contains(name);
	}
	
	void Syntax::execute(const std::vector<int>& args)
	{
		std::vector<int> newArgs(args.begin()+1, args.end());
		opcodes[args[0]].execute(newArgs);
	}
	
	void Syntax::execute(const std::string& name, std::vector<Variable>& arguments) const
	{
		methods[name].execute(arguments);
	}
	
	void Syntax::execute(const int& id)
	{
		std::vector<Variable> args;
		while(!callStack.empty())
		{
			args.push_back(getVariable([callStack.top()]));
			callStack.pop();
		}
		methods[id].execute(args);
	}
	
	std::vector<Function<Variable>> Syntax::getNativeMethods() const
	{
		return methods.getValues();
	}
	
	void Syntax::setConstants(const std::vector<Variable>& csts)
	{
		constants = csts;
	}
	
	Variable Syntax::getVariable(const int& id)
	{
		if(id == 0)
		{
			Variable result = callStack.top();
			callStack.pop();
			return result;
		}
		else
		{
			return variables[id];
		}
	}

}
