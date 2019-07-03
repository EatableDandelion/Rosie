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
		Interpreter interpreter;
		Program program = interpreter.read(fileName, syntax);
		
		ByteCodeWriter writer;
		writer.write(program);
		
		HeaderWriter headerWriter;
		headerWriter.write(program);	
	}
	
	void RosieVM::run()
	{
		HeaderReader headerReader;
		headerReader.read(state);
		std::cout << std::endl;
		
		ByteCodeReader reader(syntax);
		
		reader.read(state);
	}
}
