#include "FunctionDefinition.hpp"
#include "IncorrectTypeException.hpp"
#include "NumberOfArgumentsException.hpp"
#include "ManagedObject.hpp"
#include "TypeDefinition.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::runtime;

vector<ManagedObjectHolder> FunctionDefinition::CheckArguments(VirtualMachine* vm, CodeLocation location, vector<TypeDefinition const*> expectedTypes, vector<ManagedObject*>& actualArguments, int requiredArgumentCount) const
{
	// Check argument count first
	if (requiredArgumentCount == -1)
	{
		CheckArguments(location, expectedTypes.size(), actualArguments);
	}	
	else 
	{
		CheckArguments(location, max((unsigned)requiredArgumentCount, min(expectedTypes.size(), actualArguments.size())), actualArguments);
	}

	vector<ManagedObjectHolder> holders;
	for (vector<ManagedObject*>::size_type i = 0; i < actualArguments.size(); i++)
	{
		if (actualArguments[i]->IsStaticObject())
		{
			ErrorCode errorCode = this->GetFunctionType() == FUNCTION_TYPE_FUNCTION ? GGE2101_IncorrectNativeFunctionArgumentType : GGE2102_IncorrectOperandType;

			throw IncorrectTypeException(errorCode, location, expectedTypes[i]->GetName(), GG_STR("Static"));
		}

		if (actualArguments[i]->GetType() != expectedTypes[i])
		{
			if (expectedTypes[i]->IsConvertibleFrom(vm, actualArguments[i]->GetType()))
			{
				actualArguments[i] = expectedTypes[i]->Convert(vm, actualArguments[i]);
				holders.push_back(ManagedObjectHolder(vm, actualArguments[i]));
			}
			else 
			{
				ErrorCode errorCode = this->GetFunctionType() == FUNCTION_TYPE_FUNCTION ? GGE2101_IncorrectNativeFunctionArgumentType : GGE2102_IncorrectOperandType;

				throw IncorrectTypeException(errorCode, location, expectedTypes[i]->GetName(), actualArguments[i]->GetType()->GetName());
			}
		}
	}

	return holders;
}

void FunctionDefinition::CheckArguments(CodeLocation location, unsigned expectedArgumentCount, vector<ManagedObject*> actualArguments) const
{
	if (actualArguments.size() != expectedArgumentCount)
	{
		throw NumberOfArgumentsException(location, expectedArgumentCount, actualArguments.size());
	}
}