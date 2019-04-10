#pragma once

#include <memory>
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include "VMObjects.h"
#include "Syntax.h"

namespace Rosie
{
	
	class VirtualMachine
	{
		public:
			VirtualMachine();
			
			void execute(const std::vector<Variable>& constants, const std::vector<std::string>& commands);
			
			Handle parseHandle(const std::string& text) const;
		
		private:
			Syntax syntax;
			State state;
	};
}
