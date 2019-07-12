#include "Rosie.h"

#include "Serialization.h"

using namespace Rosie;
int main()
{
	
	//TODO: arrays and maps
	
	int exampleId = 2;
	
	if(exampleId == 1)
	{
		/** Example 1: compiling a Rosie file */
		RosieVM vm("example", Syntax());
		vm.compile();
		vm.run();
	}
	else if(exampleId == 2)
	{
		/** Example 2: Serializing and deserializing */
		Mesh mesh(Position(32.0f, "abc"));
		
		Serializer serializer("myTest");
		serializer.addMember("mesh", &mesh);
		serializer.serialize();
		
		mesh.getPosition().getX() = 1.65f;
		mesh.getPosition().getText() = "dsfd";

		serializer.deserialize();
		
		std::cout << mesh.getPosition().getText() << std::endl; // Displays "32", as first set.
	}
	
	return 0;
}