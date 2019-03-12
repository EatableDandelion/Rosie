#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include "Objects.h"

namespace Rosie
{
	class VirtualMachine
	{
		public:
			VirtualMachine();
			
			void execute(const std::vector<Variable>& constants, const std::vector<std::string>& commands);
	
			std::vector<int> split(const std::string& line);
	};
}
