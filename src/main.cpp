#include "Interpreter.h"
#include "VirtualMachine.h"
#include "Instructions.h"

using namespace Rosie;
int main()
{
	
	Interpreter c;
	Program program(c.read("test.ros"));
	VirtualMachine vm;
	ByteCodeWriter writer;
	writer.write(program);
	//vm.execute(program.getConstants(), program.getCommands());

	return 0;
}
