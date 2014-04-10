#include "LoadConstNumberInstruction.hpp"
#include "..\CodeBlockStackEntry.hpp"
#include "..\VirtualMachine.hpp"
#include "..\..\InternalErrorException.hpp"
#include "..\NumberTypeDefinition.hpp"

using namespace std;
using namespace geogen::runtime;
using namespace geogen::runtime::instructions;

InstructionStepResult LoadConstNumberInstruction::Step(VirtualMachine* vm) const
{
	NumberTypeDefinition const* numberTypeDefinition = vm->GetNumberTypeDefinition();

	DynamicObject* object = numberTypeDefinition->CreateInstance(this->constNumber);
	vm->GetMemoryManager().RegisterObject(object);
	vm->GetObjectStack().Push(object);

	return INSTRUCTION_STEP_RESULT_TYPE_NORMAL;
}