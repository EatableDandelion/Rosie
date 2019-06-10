#include "Interpreter.h"
#include "Instructions.h"

using namespace Rosie;
int main()
{
	
	//TODO define syntax that group the native methods, passed in program, interpreter and in state
	
	Syntax syntax;
	syntax.addMethod("print", [&](std::vector<Variable>& args){std::cout << args[0] << std::endl;});
	
	Interpreter interpreter;
	Program program(interpreter.read("test.ros", syntax));
	ByteCodeWriter writer("test.bc");
	writer.write(program);
	ByteCodeReader reader("test.bc", syntax);
	State state;
	reader.read(state);
	

	return 0;
}
