#pragma once

#include <utility>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <memory>
#include <functional>

namespace Rosie
{
	template<typename Key, typename T> class TreeNode;
	
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
			
			std::vector<V> getValues() const
			{
				std::vector<V> res;
				for(const auto& pair : values)
				{
					res.push_back(pair.second);
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
		
		private:
			std::unordered_map<K2, V> values;
			std::unordered_map<K1, K2> keymap;
	};
	
	/*template<typename Key, typename T>
	class TreeNode
	{
		public:
			TreeNode(const T& value):m_value(std::make_shared<T>(value))
			{}
		
			std::shared_ptr<T> operator->()
			{
				return m_value;
			}
			
			void addChild(const Key& key, const T& child)
			{
				std::shared_ptr<TreeNode<Key, T>> childNode = std::make_shared<TreeNode<Key, T>>(child);
				childNode->parent = std::weak_ptr<T>(std::make_shared<TreeNode<Key, T>>(TreeNode(m_value)));
				children.insert(std::pair<Key, std::shared_ptr<TreeNode<Key, T>>>(key, childNode));
			}
			
			std::shared_ptr<TreeNode<Key, T>> getChild(const Key& key)
			{
				return children[key];
			}
			
			std::shared_ptr<TreeNode<Key, T>> getParent()
			{
				return parent.lock();
			}
		
			std::shared_ptr<T> getValue()
			{
				return m_value;
			}
		
		private:
			std::shared_ptr<T> m_value;
			std::unordered_map<Key, std::shared_ptr<TreeNode<Key, T>>> children;
			std::weak_ptr<TreeNode<Key,T>> parent;
	};

	
	template<typename Key, typename T>
	class Scope
	{
		public:
			Scope(const T& value):m_node(std::make_shared<TreeNode<Key, T>>(value))
			{}
		
			void createScope(const Key& key, const T& value)
			{
				m_node->addChild(key, value);
			}
		
			void scopeIn(const Key& key)
			{
				m_node = m_node->getChild(key);
			}
			
			void scopeOut()
			{
				m_node = m_node->getParent();
			}
			
			std::shared_ptr<T> getValue()
			{
				return m_node->getValue();
			}
			
		private:
			std::shared_ptr<TreeNode<Key, T>> m_node;
	};*/
	
}
