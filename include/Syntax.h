#pragma once

#include <vector>
#include <unordered_map>
#include <iostream>
#include <functional>

namespace Rosie
{
	struct Opcode
	{
		public:
			Opcode(const std::string& name, const std::function<void(std::vector<Address>&)> func);
		
			void execute(const std::vector<Address>& arguments);
			std::string getName() const;
		
		private:
			std::string name;
			std::function<void(std::vector<Address>&)> func;
	};
	
	class Syntax
	{
		public:
			Syntax();
			
			void addOpcode(const std::string& name, const std::function<void(std::vector<Address>&)> func);

			void execute(const std::string& name, const std::vector<Address>& arguments);
			
		private:
			std::unordered_map<std::string, Opcode> opcodes;
	};
	
}