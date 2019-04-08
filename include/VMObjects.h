#pragma once

#include <memory>
#include <vector>
#include <string>
#include <iostream>
#include <unordered_map>
#include <stack>
#include <variant>

namespace Rosie
{
	struct Variable
	{
		public:
			Variable(const float& floatValue);
			Variable(const int& integerValue);
			Variable(const bool& booleanValue);
			Variable(const std::string& stringValue);
			Variable();

			
			template<typename T>
			T get() const
			{
				return std::get<T>(value);
			}
			
			void set(const float& newValue);
			void set(const int& newValue);
			void set(const bool& newValue);
			void set(const std::string& newValue);
			void set(const Variable& other);
			
			friend std::ostream& operator <<(std::ostream& os, Variable& var)
			{
				if(var.type == 0)
				{
					os << std::to_string(var.get<float>())+ " (float)"; 
				}
				else if(var.type == 1)
				{
					os << std::to_string(var.get<int>())+ " (int)"; 
				}
				else if(var.type == 2)
				{
					os << std::to_string(var.get<bool>())+ " (boolean)"; 
				}
				else if(var.type == 3)
				{
					os << var.get<std::string>()+ " (string)"; 
				}
				else
				{
					os << "Error: undefined variable type.";
				}
				return os;
			}
			
		private:
			int type;
			std::variant<float, int, bool, std::string> value;
	};
	
	struct Handle
	{
		public:
			Handle(const std::string& parsedArgument);
		
			int getId() const;
			Category getCategory() const;
		
		private:
			int id;
			Category category;
	};
	
	struct State
	{
		public:
			void addVariable(const int& id);
			void addConstants(const std::vector<Variable>& csts);
			void push(const Variable& variable);
			void push(const Handle& address);
			Variable pop();
			bool empty() const;
			void copyVariable(Handle& dest, const Handle& src);
			Variable getVariable(const Handle& handle);
			
		private:
			std::unordered_map<int, Variable> variables;
			std::vector<Variable> constants;
			std::stack<Variable> callStack;
	};	
}
