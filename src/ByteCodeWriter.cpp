#include <ByteCodeWriter.h>

namespace Rosie
{
	void error(const std::string& text, const Lexer& lexer)
	{
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, 12);
		std::cout << "Error line "<<std::to_string(lexer.getLineIndex())<<", at token ["<<lexer.getToken()<<"]:"<< std::endl;
		std::cout << lexer.getLine() << std::endl;
		std::cout << text << std::endl;
		SetConsoleTextAttribute(hConsole, 7);
	}
	
  	Program::Program():variables(Category::VARIABLE,1), functions(Category::FUNCTION)
	{}
	
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
		
		
		constants.push_back(Constant(type, value));
		
		addInstruction<ConstantInstruction>(constants.size()-1, constants.back());
		
		return address;//Constants have a negative index to differentiate from addresses.	
	}
	
	Address Program::newVarAddress(const std::string& name)
	{
		Address newAddress = variables.newAddress(name);
		addInstruction<NewInstruction>(name, newAddress);
		return newAddress;
	}
	
	Address Program::newVarAddress(const Token& token)
	{
		return newVarAddress(token.value);//, type);
	}
	
	Address Program::getVarAddress(const Token& token)
	{
		if(!hasVarAddress(token))
		{
			return newVarAddress(token.value);
			//std::cout << "Error, variable " << token << " undefined." <<std::endl;
		}
		return variables.getAddress(token.value);
	}
	
	bool Program::hasVarAddress(const Token& token)
	{
		return variables.hasAddress(token.value);
	}
	
	Address Program::newFunctionAddress(const std::string& name)
	{
		return functions.newAddress(name);
	}
	
	Address Program::getFunctionAddress(const Token& token, const Lexer& lexer) const
	{
		if(!hasFunctionAddress(token))
		{
			Rosie::error("Error, function " + token.getString() + " undefined.", lexer);
		}
		return functions.getAddress(token.value);
	}
	
	bool Program::hasFunctionAddress(const Token& token) const
	{
		return functions.hasAddress(token.value);
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
	
	std::vector<std::string> Program::getCommands() const
	{
		return instructions.getCommands();
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

	ByteCodeWriter::ByteCodeWriter(const std::string& fileName):fileName(fileName)
	{}
	
	void ByteCodeWriter::write(const Program& program) const
	{	
		std::ofstream file;
		file.open(fileName);
		
		for(std::string command : program.getCommands())
		{
			file << command << std::endl;
		}
		
		file.close();
	}
	
	ByteCodeReader::ByteCodeReader(const std::string& fileName, const Syntax& syntax):fileName(fileName)
	{
		addInstruction<SetInstruction>();
		addInstruction<NewInstruction>();
		addInstruction<ConstantInstruction>();
		addInstruction<AddInstruction>();
		addInstruction<ArgumentInstruction>();
		addInstruction<CallInstruction>(syntax);
	}
	
	void ByteCodeReader::read(State& state) const
	{
		std::string command;
		std::ifstream file(fileName);
		int instructionId = 0;
		if(file.is_open())
		{
			while(getline(file,command))
			{
			  	instructionId = std::stoi(command.substr(std::size_t(0), command.find("|", std::size_t(0))));

				if(instructions.find(instructionId) != instructions.end())
				{
					std::cout << command << std::endl;
					std::cout << instructions.at(instructionId)->getName() << std::endl;
					std::cout << std::endl;
					instructions.at(instructionId)->read(command.substr(command.find("|", std::size_t(0))+1, command.size()), state);
				}
				else
				{
					std::cout << "Instruction "+std::to_string(instructionId)+" unknown." << std::endl;
				}
			}
			file.close();
		}

	}
}
