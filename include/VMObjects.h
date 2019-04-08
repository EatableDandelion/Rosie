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
			void addMember(const Variable& member);
			std::shared_ptr<Variable> getMember(const int& index) const;
			
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
				else if(var.type == 4)
				{
					os << "[";
					for(const auto& member : var.members)
					{
						os << *member << ", "
					}
					os << "]"
				}
				return os;
			}
			
		private:
			int type;
			std::variant<float, int, bool, std::string> value;
			std::vector<std::shared_ptr<Variable>> members;
	};
	
	struct Handle
	{
		public:
			Handle(const std::vector<int>& id, const Category& category);
		
			std::vector<int> getId() const;
			Category getCategory() const;
			bool operator==(const Handle& other);
		
		private:
			std::vector<int> id;
			Category category;
	};
	
	struct State
	{
		public:
			void addVariable(const Handle& handle);
			void addConstants(const std::vector<Variable>& csts);
			void push(const Variable& variable);
			void push(const Handle& address);
			Variable pop();
			bool empty() const;
			void copyVariable(Handle& dest, const Handle& src);
			Variable getVariable(const Handle& handle);
			
		private:
			std::unordered_map<Handle, Variable> variables;
			std::vector<Variable> constants;
			std::stack<Variable> callStack;
	};	
}
