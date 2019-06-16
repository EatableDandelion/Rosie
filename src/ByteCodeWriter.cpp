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
		address.setType(type);
		
		constants.push_back(Constant(type, value));
		
		addInstruction<ConstantInstruction>(constants.size()-1, constants.back());
		
		return address;//Constants have a negative index to differentiate from addresses.	
	}
	
	Address Program::newVarAddress(const std::string& name, const TokenType& type)
	{
		Address newAddress = variables.newAddress(name, type);
		addInstruction<NewInstruction>(name, newAddress);
		return newAddress;
	}
	
	Address Program::newVarAddress(const Token& token, const TokenType& type)
	{
		return newVarAddress(token.value, type);
	}
	
	Address Program::getVarAddress(const Token& token)
	{
		if(!hasVarAddress(token))
		{
			//return newVarAddress(token.value);
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
		return functions.newAddress(name);
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
	
	Address Program::getStackAddress(const TokenType& type)
	{
		Address res = getStackAddress();
		res.setType(type);
		return res;
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
	
	std::vector<Constant> Program::getConstants() const
	{
		return constants;
	}
		
	std::vector<Address> Program::getVariables() const
	{
		return variables.getAddresses();
	}
		
	std::vector<Address> Program::getFunctions() const
	{
		return functions.getAddresses();
	}

	
	
	HeaderWriter::HeaderWriter(const std::string& fileName):fileName(fileName)
	{}
	
	void HeaderWriter::write(const Program& program)
	{	
		std::ofstream file;
		file.open(fileName);
		
		for(Constant constant : program.getConstants())
		{
			file << 0 << " " << constant.getValue() << " " << constant.getTypeId() << std::endl;
		}
		
		for(Address variable : program.getVariables())
		{
			file << 1 << " " << variable.getName() << " " << variable.getId() << " " << variable.getTypeId() << std::endl;
		}
		
		for(Address function : program.getFunctions())
		{
			file << 2 << " " << function.getName() << " " << function.getId() << std::endl;
		}
		
		file.close();
	}
	
	
	
	HeaderReader::HeaderReader(const std::string& fileName):fileName(fileName), cstIndex(0)
	{}
	
	void HeaderReader::read(State& state)
	{
		std::string line;
		std::ifstream file(fileName);
		int typeId = 0;
		if(file.is_open())
		{
			while(getline(file,line))
			{
				int category;
				char name[20];
				int int1;
				int int2;
				sscanf(line.c_str(), "%d %s %d %d", &category, &name, &int1, &int2);
				
				if(category == 0) // It's a constant
				{
					defineConstant(state, std::string(name), int1);
				}
				else if(category == 1) // It's a variable
				{
					defineVariable(state, std::string(name), int1, int2);
				}
				else if(category == 2) // It's a function
				{
					defineFunction(state, std::string(name), int1);
				}
				
				std::cout << line << std::endl;
			}
			file.close();
		}

	}
	
	void HeaderReader::defineConstant(State& state, const std::string& value, const int& typeId)
	{
		TokenType type = TokenType(typeId);

		if(type == TokenType::CSTINT)
		{
			state.addConstant(cstIndex, Variable(std::stoi(value)));
		}
		else if(type == TokenType::CSTFLOAT)
		{
			state.addConstant(cstIndex, Variable(std::stof(value)));
		}
		else if(type == TokenType::CSTBOOLEAN)
		{
			if(value == "true")
			{
				state.addConstant(cstIndex, Variable(true));
			}
			else
			{
				state.addConstant(cstIndex, Variable(false));
			}
		}
		else if(type == TokenType::CSTSTRING)
		{
			state.addConstant(cstIndex, Variable(value));
		}
		cstIndex++;
	}
	
	void HeaderReader::defineVariable(State& state, const std::string& name, const int& id, const int& typeId) const
	{
		state.addVariable(name, typeId, Handle(id, Category::VARIABLE));
	}
	
	void HeaderReader::defineFunction(State& state, const std::string& name, const int& id) const
	{
		state.addFunction(id, name);
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
