#include "Interpreter.h"
#include "VirtualMachine.h"
#include "Instructions.h"

using namespace Rosie;
int main()
{
	
	//Syntax syntax;
	
	Interpreter c;
	Program program(c.read("test.ros"));
	VirtualMachine vm;
	ByteCodeWriter writer("test.bc");
	writer.write(program);
	ByteCodeReader reader("test.bc");
	State state;
	reader.read(state);
	//vm.execute(program.getConstants(), program.getCommands());

	return 0;
}
