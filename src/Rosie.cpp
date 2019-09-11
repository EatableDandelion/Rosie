#include "Rosie.h"

namespace Rosie{
	RosieVM::RosieVM(const std::string& fileName, const Syntax& syntaxInput):fileName(fileName), state(fileName), syntax(syntaxInput)
	{
		state.setFunction("print", [&](CallStack& stack){	
			while(!stack.empty())
			{
				Variable v = stack.pop();
				std::cout << v << " ";
			}
		std::cout << std::endl;});
		
		state.setFunction("+", [&](CallStack& stack){stack.push(stack.pop()+stack.pop());});
		state.setFunction("-", [&](CallStack& stack){stack.push(stack.pop()-stack.pop());});
		state.setFunction("*", [&](CallStack& stack){stack.push(stack.pop()*stack.pop());});
		state.setFunction("/", [&](CallStack& stack){stack.push(stack.pop()/stack.pop());});
		state.setFunction("u+", [&](CallStack& stack){});
		state.setFunction("u-", [&](CallStack& stack){stack.push(-stack.pop());});
		
		syntax.setNativeFunctions(state.getFunctionsList());
	}
	
	void RosieVM::compile()
	{
		try{
			Interpreter interpreter;
			Program program = interpreter.read(fileName, syntax);
			
			ByteCodeWriter declarationWriter(fileName, ".tc");
			declarationWriter.write(program->getHeader());
			
			ByteCodeWriter instructionWriter(fileName, ".bc");
			instructionWriter.write(program->getCommands());
		}
		catch(const std::exception& error)
		{
			std::cerr << error.what();
		}
	
	}
	
	void RosieVM::run()
	{	
		try{
			ByteCodeReader headerReader(".tc");
			
			headerReader.addInstruction<ConstantHeader>();
			headerReader.addInstruction<VariableHeader>();
			headerReader.addInstruction<FunctionHeader>();
			//headerReader.addInstruction<ScopeInstruction>();
			headerReader.read(state);
			
			std::cout << "=================================" << std::endl;
			
			ByteCodeReader instructionReader(".bc", true);
			
			instructionReader.addInstruction<SetInstruction>();
			instructionReader.addInstruction<ArgumentInstruction>();
			instructionReader.addInstruction<CallInstruction>(syntax);
			//instructionReader.addInstruction<ScopeInstruction>();
			
			instructionReader.read(state);
			
		}
		catch(const std::exception& error)
		{
			std::cerr << error.what();
		}
	}
	
	State RosieVM::getState() const
	{
		return state;
	}
}
