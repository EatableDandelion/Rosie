#include <ByteCodeWriter.h>

namespace Rosie
{
  	Program::Program():variables(Category::VARIABLE,1), functions(Category::FUNCTION)
	{
		for(const auto& func : syntax.getNativeMethods())
		{
			functions.newAddress(func.getName(), 1);
		}
	}
	
	Address Program::getAddress(const Token& token, const Lexer& lexer)
	{
		if(hasFunctionAddress(token))
		{
			return getFunctionAddress(token, lexer);
		}
		else if(hasVarAddress(token))
		{
			return getVarAddress(token);
		}
		else
		{
			return newCstAddress(token);
		}
	}
	
	Address Program::newCstAddress(const Token& token)
	{
		std::string value = token.value;
		TokenType type = token.type;
		
		Address address(constants.size(), Category::CONSTANT, value);
		
		
		
		if(type == TokenType::CSTFLOAT)
		{
			constants.push_back(Constant("float", value));
		}
		else if(type == TokenType::CSTINT)
		{
			constants.push_back(Constant("int", value));
		}
		else if(type == TokenType::CSTBOOLEAN)
		{
			constants.push_back(Constant("boolean", value));
		}
		else if(type == TokenType::CSTSTRING)
		{
			constants.push_back(Constant("string", value));
		}
		else
		{
			std::cout << "Constant type undefined" << std::endl;
		}
		
		
		return address;//Constants have a negative index to differentiate from addresses.	
	}
	
	Address Program::newVarAddress(const std::string& name, const Type& type)
	{
		return variables.newAddress(name, types.getSize(type));
	}
	
	Address Program::newVarAddress(const Token& token, const Type& type)
	{
		return newVarAddress(token.value, type);
	}
	
	Address Program::getVarAddress(const Token& token)
	{
		if(!hasVarAddress(token))
		{
			
			std::cout << "Error, variable " << token << " undefined." <<std::endl;
		}
		return variables.getAddress(token.value);
	}
	
	bool Program::hasVarAddress(const Token& token)
	{
		return variables.hasAddress(token.value);
	}
	
	Address Program::newFunctionAddress(const std::string& name)
	{
		return functions.newAddress(name, 1);
	}
	
	Address Program::getFunctionAddress(const Token& token, const Lexer& lexer)
	{
		if(!hasFunctionAddress(token))
		{
			Rosie::error("Error, function " + token.getString() + " undefined.", lexer);
		}
		return functions.getAddress(token.value);
	}
	
	bool Program::hasFunctionAddress(const Token& token)
	{
		return functions.hasAddress(token.value);
	}
	
	bool Program::isConstructor(const Token& token)
	{
		return hasTypeName(token);
	}
	
	void Program::addMemberToType(const Type& type, const std::string& memberName, const std::string& memberType)
	{
		types.addMemberToType(type, memberName, memberType);
	}
	
	void Program::startScope()
	{
		std::cout << "Starting scope" << std::endl;
		variables.startScope();
	}
	
	void Program::endScope()
	{
		std::cout << "End scope" << std::endl;
		variables.endScope();
	}
	
	std::vector<Variable> Program::getConstants() const
	{
		return constants;
	}
	
	std::vector<std::string> Program::getCommands() const
	{
		return instructions;
	}
	
	Address Program::getStackAddress() const
	{
		return Address(0, Category::VARIABLE);
	}
	
	Type Program::addType(const std::string& name)
	{
		types.addType(name);
		return getType(name);
	}
	
	Type Program::getType(const std::string& name) const
	{
		return types.getType(name);
	}
	
	Type Program::getType(const Token& token) const
	{
		return getType(token.value);
	}
	
	bool Program::hasTypeName(const Token& token) const
	{
		return types.hasType(token.value);
	}
}
