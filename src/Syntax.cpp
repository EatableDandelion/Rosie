#include "Syntax.h"

namespace Rosie
{

	Syntax::Syntax()
	{	
		addOpcode(Opcode::SET, [&](std::vector<Handle>& args, State& state){state.copyVariable(args[0], args[1]);});
		addOpcode(Opcode::ARG, [&](std::vector<Handle>& args, State& state){state.push(args[0]);});
		addOpcode(Opcode::CALL, [&](std::vector<Handle>& args, State& state){execute(state.getVariable(args[0]).get<int>(), state);});
		addOpcode(Opcode::ADD, [&](std::vector<Handle>& args, State& state){state.push(Variable(state.getVariable(args[0]).get<float>()+state.getVariable(args[1]).get<float>()));});
		addOpcode(Opcode::NEG, [&](std::vector<Handle>& args, State& state){state.push(Variable(-state.getVariable(args[0]).get<float>()));});
		addOpcode(Opcode::SUB, [&](std::vector<Handle>& args, State& state){state.push(Variable(state.getVariable(args[0]).get<float>()-state.getVariable(args[1]).get<float>()));});
		addOpcode(Opcode::MULT, [&](std::vector<Handle>& args, State& state){state.push(Variable(state.getVariable(args[0]).get<float>()*state.getVariable(args[1]).get<float>()));});
		addOpcode(Opcode::DIV, [&](std::vector<Handle>& args, State& state){state.push(Variable(state.getVariable(args[0]).get<float>()/state.getVariable(args[1]).get<float>()));});
		
		addMethod("print", [&](std::vector<Variable>& args, State& state){std::cout << args[0] << std::endl;});
		//addMethod("Position", [&](std::vector<Variable>& args, State& state){});
	}
	
	int Syntax::getOpcodeId(const Opcode& name) const
	{
		return opcodes[name].getId();
	}
	
	void Syntax::addOpcode(const Opcode& name, const std::function<void(std::vector<Handle>&, State&)> func)
	{
		opcodes.add(opcodes.size(), name, Function<Handle>(std::to_string(name), func, opcodes.size()));
	}
	
	void Syntax::addMethod(const std::string& name, const std::function<void(std::vector<Variable>&, State&)> func)
	{
		methods.add(methods.size(), name, Function<Variable>(name, func, methods.size()));
	}
	
	bool Syntax::hasMethod(const std::string& name) const
	{
		return methods.contains(name);
	}
	
	void Syntax::runOpcode(const int& id, std::vector<Handle>& args, State& state)
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
