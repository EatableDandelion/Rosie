#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <set>

namespace Rosie
{
	enum Functions
	{
		
		SET = 0x00,
		GET = 0x01,
		ADD = 0x02
	};
	
	struct Command
	{
		Functions func;
		std::vector<int> args;
	};
	
	class VirtualMachine
	{
		public:
			VirtualMachine();
			
			void execute();

		private:
			std::vector<Command> commands;
			std::set<float> variables;
	};
}
