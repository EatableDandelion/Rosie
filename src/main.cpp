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
	
	State state(syntax);
	HeaderReader headerReader("test.hc");
	headerReader.read(state);
	std::cout << std::endl;
	ByteCodeReader reader("test.bc", syntax);
	state.setFunction("print", [&](std::vector<Variable>& args, std::stack<Variable>& results){std::cout << args[0] << std::endl;});
	state.setFunction("-", [&](std::vector<Variable>& args, std::stack<Variable>& results){results.push(args[0].get<float>()-args[1].get<float>());});

	reader.read(state);
	
	
	return 0;
}
