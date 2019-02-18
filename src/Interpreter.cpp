#include "Interpreter.h"

namespace Rosie{
	
	Interpreter::Interpreter()	
	{
		//addFunction()
	}
	
	void Interpreter::read(const std::string& fileName)
	{
		Lexer stream(fileName);
		//std::vector<Token> tokens;
		std::shared_ptr<Instruction> root = std::make_shared<Instruction>();
		std::shared_ptr<Instruction> instruction = std::make_shared<Instruction>(root);
		
		stream.getTokens(instruction);
		instruction->getRoot()->print();
		/*for(Token token : tokens)
		{
			std::cout << token.value << "\t \t \t " << token.type << std::endl;			
		}*/
	}

}
