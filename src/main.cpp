#include "Interpreter.h"
#include "VirtualMachine.h"

int main()
{
	
	Rosie::Interpreter c;
	Rosie::Program program(c.read("test.ros"));
	Rosie::VirtualMachine vm;
	vm.execute(program.getConstants(), program.getCommands());
	return 0;
}
