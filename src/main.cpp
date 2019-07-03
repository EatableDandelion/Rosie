#include "Rosie.h"

using namespace Rosie;
int main()
{
	
	RosieVM vm("test", Syntax());
	vm.compile();
	vm.run();
	
	std::cout << vm.getValue<float>("a") << std::endl;
	
	return 0;
}
