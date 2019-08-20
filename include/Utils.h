#pragma once

#include <utility>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <memory>
#include <functional>
#include <windows.h>

namespace Rosie
{
	std::size_t getId(const std::string& name);
	
	std::vector<std::string> split(const std::string& line, const std::string& separator);
	
	template<typename K1, typename K2, typename V>
	class DualMap //map that can be searched for either K1 OR K2
	{
		public:
			void add(const K1& key1, const K2& key2, const V& value)
			{
				values.insert(std::pair<K2,V>(key2, value));
				keymap.insert(std::pair<K1,K2>(key1, key2));
			}
		
			bool contains(const K1& key1) const
			{
				return keymap.find(key1) != keymap.end();
			}
		
			bool contains(const K2& key2) const
			{
				return values.find(key2) != values.end();
			}
		
			V operator[](const K1& key1) const
			{
				return values.at(keymap.at(key1));
			}
		
			V operator[](const K2& key2) const
			{
				return values.at(key2);
			}
			
			V& operator[](const K1& key1)
			{
				return values[keymap.at(key1)];
			}
			
			V& operator[](const K2& key2)
			{
				return values[key2];
			}
			
			int size() const
			{
				return values.size();
			}
		
			void erase(const K1& key1)
			{
				values.erase(keymap.at(key1));
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
			
			std::vector<K1> getKey1() const
			{
				std::vector<K1> res;
				for(const auto& pair : keymap)
				{
					res.push_back(pair.first);
				}
				return res;
			}
			
			std::vector<K2> getKey2() const
			{
				std::vector<K2> res;
				for(const auto& pair : values)
				{
					res.push_back(pair.first);
				}
				return res;
			}
			
			std::vector<V> getValues() const
			{
				std::vector<V> res;
				for(const auto& pair : values)
				{
					res.push_back(pair.second);
				}
				return res;
			}
		
		private:
			std::unordered_map<K2, V> values;
			std::unordered_map<K1, K2> keymap;
	};
	
	class ErrorDisplay
	{
		public:
			ErrorDisplay();
			~ErrorDisplay();
			
			void show(const std::string& msg, const bool& warningOn = true) const;
			
		private:
			void start() const;
			void end() const;
	};
	
	class BaseError : public std::exception
	{
		public:
			BaseError(const std::string& msg);
			
			const char* what() const throw();
			
		private:
			const std::string msg;
	};

}
