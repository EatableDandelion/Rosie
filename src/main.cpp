#include "Interpreter.h"
#include "VirtualMachine.h"
#include "Instructions.h"
int main()
{
	
	Rosie::Interpreter c;
	Rosie::Program program(c.read("test.ros"));
	Rosie::VirtualMachine vm;
	//vm.execute(program.getConstants(), program.getCommands());
	Rosie::InstructionCollection coll;
	coll.addInstruction<Rosie::ConstantInstruction>(0, Rosie::Constant("aa","aa"));
	coll.addInstruction<Rosie::TestInstruction>();
	return 0;
}
