#include "Interpreter.h"
#include "Instructions.h"

using namespace Rosie;
int main()
{
	
	//TODO write real bytecode, with header to give the names of constants, functions and variables, and exec code later
	
	Syntax syntax;
	
	
	Interpreter interpreter;
	Program program(interpreter.read("test.ros", syntax));
	ByteCodeWriter writer("test.bc");
	writer.write(program);
	HeaderWriter headerWriter("test.hc");
	headerWriter.write(program);
	
	State state;
	HeaderReader headerReader("test.hc");
	headerReader.read(state);
	std::cout << std::endl;
	
	ByteCodeReader reader("test.bc", syntax);
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

	reader.read(state);
	
	
	return 0;
}
