#pragma once

#include "Interpreter.h"
#include "Instructions.h"

namespace Rosie{	
	class RosieVM
	{
		public:
			RosieVM(const std::string& fileName, const Syntax& syntax);
			
			void compile();
			
			void run();
			
			State getState() const;
			
		private:
			std::string fileName;
			State state;
			Syntax syntax;
	};
}
