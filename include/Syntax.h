#pragma once

#include <vector>
#include <unordered_map>
#include <iostream>
#include <functional>
#include <stack>
#include "Objects.h"

namespace Rosie
{
	struct Opcode
	{
		public:
			Opcode(const std::string& name, const std::function<void(std::vector<int>&)> func, const int& id);
			Opcode(Opcode&& other);
		
			void execute(std::vector<int>& arguments) const;
			std::string getName() const;
			int getId() const;
		
		private:
			std::string name;
			int id;
			std::function<void(std::vector<int>&)> func;
	};
	
	class Syntax
	{
		public:
			Syntax();
			
			int getOpcodeId(const std::string& name) const;
			
			void addOpcode(const std::string& name, const std::function<void(std::vector<int>&)> func);

			void execute(const std::string& name, std::vector<int>& arguments) const;
			
			void execute(const int& id);
			
		private:
			std::unordered_map<std::string, Opcode> opcodes;
			std::unordered_map<int, Variable> variables;
			std::unordered_map<int, Variable> constants;
			std::stack<int> callStack;
	};
	
	
}
