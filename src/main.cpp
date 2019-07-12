#include "Rosie.h"

#include "Serialization.h"

using namespace Rosie;
int main()
{
	
	RosieVM vm("test", Syntax());
	vm.compile();
	vm.run();
	Mesh mesh(Position(1.65f));
	
	Deserializer deserializer;
	deserializer.addMember("mesh", &mesh);
	deserializer.run(vm);
	
	
	std::cout << mesh.getPosition().getX() << std::endl;

	return 0;
}