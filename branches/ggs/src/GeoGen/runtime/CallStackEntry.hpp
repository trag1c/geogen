#pragma once

#include <vector>
#include <stack>
#include <map>

#include "../Serializable.hpp"
#include "CodeBlock.hpp"
#include "CodeBlockStackEntry.hpp" 
#include "FunctionDefinition.hpp"
//#include "DynamicObject.hpp"
#include "instructions/Instruction.hpp"

namespace geogen
{
	namespace runtime
	{
		class DynamicObject;
		class VirtualMachine;

		enum CallStackEntryStepResult
		{
			CALL_STACK_ENTRY_STEP_RESULT_RUNNING,
			CALL_STACK_ENTRY_STEP_RESULT_FINISHED
		};

		class CallStackEntry : public Serializable
		{
		private:
			FunctionDefinition const* functionDefinition;
			std::stack<CodeBlockStackEntry> codeBlockStack;
			std::map<std::string, DynamicObject*> localVariableValues;
		public:
			CallStackEntry(CallStackEntry const& other);
			CallStackEntry& operator=(CallStackEntry const& other);

			CallStackEntry(FunctionDefinition const* functionDefinition) : functionDefinition(functionDefinition) {};
			~CallStackEntry() {};

			inline std::stack<CodeBlockStackEntry>& GetCodeBlockStack() { return this->codeBlockStack; };

			void CallCodeBlock(CodeBlock const& codeBlock, bool isLooping);

			CallStackEntryStepResult Step(VirtualMachine* vm);

			//typedef std::vector<instructions::Instruction const*>::const_iterator const_iterator;

			//void AddInstruction(instructions::Instruction const* instruction);
			//void MoveInstructionsFrom(CodeBlock& another);

			//inline const_iterator Begin() const { return this->instructions.begin(); }
			//inline const_iterator End() const { return this->instructions.end(); }

			virtual void Serialize(std::iostream& stream) const;
		};
	}
}