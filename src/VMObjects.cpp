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
	{
		type = 4;
	}
	
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
	
	void Variable::addMember(const Variable& member)
	{
		members.push_back(std::make_shared<Variable>(member));
	}
	
	
	std::shared_ptr<Variable> Variable::getMember(const int& index) const
	{
		return members[index];
	}
	
	Handle::Handle(const std::vector<int>& id, const Category& category):id(id), category(category)
	{}
	
	std::vector<int> Handle::getId() const
	{
		return id;
	}
	
	Category Handle::getCategory() const
	{
		return category;
	}
	
	bool operator==(const Handle& other)
	{
		return id = other.id;
	}
	
	void State::addVariable(const Handle& handle)
	{
		variables.insert(std::pair<Handle, Variable>(handle, Variable()));
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
			if(handle.getId()[0] == 0)
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
