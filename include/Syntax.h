#pragma once

#include <vector>
#include <unordered_map>
#include <iostream>
#include <functional>
#include <stack>
#include "Objects.h"

namespace Rosie
{
	template<typename T>
	struct Function
	{
		public:
			Function(const std::string& name, const std::function<void(std::vector<T>&)> func, const int& id):name(name), id(id)
			{
				m_func = func;
			}
			
			Function(const Function<T>& other):name(other.name), m_func(other.m_func), id(other.id)
			{}
		
			void execute(std::vector<T>& arguments) const
			{
				m_func(arguments);
			}
	
			std::string getName() const
			{
				return name;
			}
	
			int getId() const
			{
				return id;
			}
		
		private:
			std::string name;
			int id;
			std::function<void(std::vector<T>&)> m_func;
	};
	
	template<typename K1, typename K2, typename V>
	class DualMap //map that can be searched for either K1 OR K2
	{
		public:
			void add(const K1&, key1 const K2& key2, const V& value)
			{
				values.insert(std::pair<K2,V>(key2, value));
				keymap.insert(std::pair<K1,K2>(key1, key2));
			}
		
			bool contains(const K1& key1)
			{
				return keymap.find(key1) != keymap.end();
			}
		
			bool contains(const K2& key2)
			{
				return values.find(key2) != values.end();
			}
		
			V operator[](const K1& key1) const
			{
				return values[keymap[key1]];
			}
		
			V operator[](const K2& key2) const
			{
				return values[key2];
			}
		
			void erase(const K1& key1)
			{
				values.erase(keymap[key1]);
				keymap.erase(key1);
			}
		
			void erase(const K2& key2)
			{
				values.erase(key2);
				for(const auto& it : keymap)
				{
					if(it.second == key2)
					{
						keymap.erase(it.first);
					}
				}
			}
		
		private:
			std::unordered_map<K2, V> values;
			std::unordered_map<K1, K2> keymap;
	};
	
	class Syntax
	{
		public:
			Syntax();
			
			int getOpcodeId(const std::string& name) const;
			
			void addOpcode(const std::string& name, const std::function<void(std::vector<int>&)> func);

			void addMethod(const std::string& name, const std::function<void(std::vector<Variable>&)> func);
			
			bool hasMethod(const std::string& name) const;
			
			void execute(const std::vector<int>& args);
			
			void execute(const std::string& name, std::vector<Variable>& arguments) const;
			
			void execute(const int& id);
			
			std::unordered_map<std::string, Function<Variable>> getNativeMethods() const;
			
		private:
			DualMap<int, std::string, Function<int>> opcodes;
			std::unordered_map<std::string, Function<Variable>> methods;
			std::unordered_map<int, Variable> variables;
			std::unordered_map<int, Variable> constants;
			std::stack<int> callStack;
	};
	
	
}
