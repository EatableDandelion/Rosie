#include "Syntax.h"

namespace Rosie
{

	Syntax::Syntax()
	{	
	
		addOpcode("NEW", [&](std::vector<Address>& args, State& state){});
		addOpcode("SET", [&](std::vector<Address>& args, State& state){
				state.copyVariable(args[1], args[0]);
			});
		addOpcode("ARG", [&](std::vector<Address>& args, State& state){state.push(args[0]);});
		addOpcode("PRINT", [&](std::vector<Address>& args, State& state){std::cout << state.getVariable(args[0]) << std::endl;});
		addOpcode("CALL", [&](std::vector<Address>& args, State& state){execute(state.getVariable(args[0]).get<int>(), state);});
		addOpcode("ADD", [&](std::vector<Address>& args, State& state){state.push(Variable(state.getVariable(args[0]).get<float>()+state.getVariable(args[1]).get<float>()));});
		addOpcode("NEG", [&](std::vector<Address>& args, State& state){state.push(Variable(-state.getVariable(args[0]).get<float>()));});
		addOpcode("SUB", [&](std::vector<Address>& args, State& state){state.push(Variable(state.getVariable(args[0]).get<float>()-state.getVariable(args[1]).get<float>()));});
		addOpcode("MULT", [&](std::vector<Address>& args, State& state){state.push(Variable(state.getVariable(args[0]).get<float>()*state.getVariable(args[1]).get<float>()));});
		addOpcode("DIV", [&](std::vector<Address>& args, State& state){state.push(Variable(state.getVariable(args[0]).get<float>()/state.getVariable(args[1]).get<float>()));});
		
		addMethod("print", [&](std::vector<Variable>& args, State& state){std::cout << args[0] << std::endl;});
		//addMethod("Position", [&](std::vector<Variable>& args, State& state){});
	}
	
	int Syntax::getOpcodeId(const std::string& name) const
	{
		if(!opcodes.contains(name)) std::cout << "Opcode "+name+" unrecognized" << std::endl;
		return opcodes[name].getId();
	}
	
	void Syntax::addOpcode(const std::string& name, const std::function<void(std::vector<Address>&, State&)> func)
	{
		opcodes.add(opcodes.size(), name, Function<Address>(name, func, opcodes.size()));
	}
	
	void Syntax::addMethod(const std::string& name, const std::function<void(std::vector<Variable>&, State&)> func)
	{
		methods.add(methods.size(), name, Function<Variable>(name, func, methods.size()));
	}
	
	bool Syntax::hasMethod(const std::string& name) const
	{
		return methods.contains(name);
	}
	
	void Syntax::runOpcode(const int& id, std::vector<Address>& args, State& state)
	{
		opcodes[id].execute(args, state);
	}
	
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
	}
	
	std::vector<Function<Variable>> Syntax::getNativeMethods() const
	{
		return methods.getValues();
	}

}
