#include "VirtualMachine.h"

namespace Rosie
{
	VirtualMachine::VirtualMachine()	
	{}
	
	void VirtualMachine::execute()
	{
		for(Command command : commands)
		{
			switch(command.func)
			{
				case 0x00 :
					break;
					
				case 0x01 :
					break;
					
				case 0x02 :
					break;
			}
		}
	}
}
