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
	
	Variable::Variable(const TokenType& tokenType)
	{
		if(tokenType == TokenType::CSTFLOAT)
		{
			type = 0;
		}
		else if(tokenType == TokenType::CSTINT)
		{
			type = 1;
		}
		else if(tokenType == TokenType::CSTBOOLEAN)
		{
			type = 2;
		}
		else if(tokenType == TokenType::CSTSTRING)
		{
			type = 3;
		}
		else
		{
			type = 4;
		}
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
	
	void Variable::addMember(const std::string& name, const Variable& member)
	{
		members.insert(std::pair<std::size_t, std::shared_ptr<Variable>>(Rosie::getId(name), std::make_shared<Variable>(member)));
	}
	
	
	std::shared_ptr<Variable> Variable::getMember(const std::string& name) const
	{
		return members.at(Rosie::getId(name));
	}
	
	Handle::Handle(const int& id, const Category& category):id(id), category(category)
	{}
	
	int Handle::getId() const
	{
		return id;
	}
	
	Category Handle::getCategory() const
	{
		return category;
	}
	
	bool Handle::operator==(const Handle& other) const
	{
		return id == other.id;
	}
	
	State::State(const Syntax& syntax):syntax(syntax)
	{}
	
	void State::addVariable(const std::string& name, const int& tokenType, const Handle& handle)
	{
		variables.add(handle, Rosie::getId(name), Variable(TokenType(type)));
	}
	
	void State::addConstant(const int& id, const Variable& csts)
	{
		constants[id] = csts;
	}
	
	void State::addMethod(const Function<Variable>& func)
	{
		methods.insert(std::pair<std::string, Function<Variable>> (func.getName(), func));
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
	
	/*bool State::empty() const
	{
		return callStack.empty();
	}*/
	
	void State::copyVariable(Handle& dest, const Handle& src)
	{
		variables[dest] = getVariable(src);
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
				if(!variables.contains(handle))
				{
					variables.add(handle, std::size_t(0), Variable());
				}
				return variables[handle];				
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
	
	void State::execute(const std::string& methodName)
	{
		std::vector<Variable> args;
		while(!callStack.empty())
		{
			args.push_back(pop());
		}

		methods[methodName].execute(args, callStack);	
	}
}
