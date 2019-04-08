#include "VMObjects.h"

namespace Rosie
{
	Variable::Variable(const float& floatValue)
	{
		set(floatValue);
	}

	Variable::Variable(const int& integerValue)
	{
		set(integerValue);
	}
	
	Variable::Variable(const bool& booleanValue)
	{
		set(booleanValue);
	}
	
	Variable::Variable(const std::string& stringValue)
	{
		set(stringValue);
	}
	
	Variable::Variable()
	{}
	
	void Variable::set(const float& newValue)
	{
		value = newValue;
		type = 0;
	}
	
	void Variable::set(const int& newValue)
	{
		value = newValue;
		type = 1;
	}
	
	void Variable::set(const bool& newValue)
	{
		value = newValue;
		type = 2;
	}
	
	void Variable::set(const std::string& newValue)
	{
		value = newValue;
		type = 3;
	}
	
	void Variable::set(const Variable& other)
	{
		value = other.value;
		type = other.type;
	}
	
	Handle::Handle(const int& id, const Category& category, const Variable& variable)
	{
		std::size_t sepPos = line.find("/", std::size_t(0));
		std::string value = line.substr(startPos, sepPos-startPos)
		//addressId = std::stoi();
		sepPos++;
		category = std::stoi(line.substr(sepPos, endPos-sepPos));
	}
	
	int Handle::getId() const
	{
		return id;
	}
	
	Category Handle::getCategory() const
	{
		return category;
	}
	
	void State::addVariable(const int& id)
	{
		variables.insert(std::pair<int, Variable>(id, Variable()));
	}
	
	void State::addConstants(const std::vector<Variable>& csts)
	{
		constants = csts;
	}
	
	void State::push(const Variable& variable)
	{
		callStack.push(variable);
	}
	
	void State::push(const Handle& handle)
	{
		push(getVariable(handle));
	}
	
	Variable State::pop()
	{
		Variable res = callStack.top();
		callStack.pop();
		return res;
	}
	
	bool State::empty() const
	{
		return callStack.empty();
	}
	
	void State::copyVariable(Handle& dest, const Handle& src)
	{
		variables[dest.getId()] = getVariable(src);
	}
	
	Variable State::getVariable(const Handle& handle)
	{
		if(handle.getCategory() == Category::CONSTANT)
		{
			return constants[handle.getId()];			
		}
		else if(handle.getCategory() == Category::VARIABLE)
		{
			if(handle.getId() == 0)
			{
				return pop();
			}
			else
			{
				if(variables.find(handle.getId()) == variables.end())
				{
					variables.insert(std::pair<int, Variable>(handle.getId(), Variable()));
				}
				return variables[handle.getId()];				
			}
		}
		else if(handle.getCategory() == Category::INTEGER)
		{
			return Variable(handle.getId());
		}
		else //to be implemented
		{
			return Variable(handle.getId());
		}
	}
}
