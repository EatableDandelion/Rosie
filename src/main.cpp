#include "Interpreter.h"

int main()
{
	Rosie::Interpreter c;
	Rosie::Program program(c.read("test.ros"));
	
	return 0;
}
