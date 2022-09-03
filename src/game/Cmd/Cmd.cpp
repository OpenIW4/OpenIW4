#include "Cmd.hpp"
#include "../Com/Com.hpp"


//DONE : 0x4A5510
cmd_function_s* Cmd_FindCommand(const char* cmdName)
{
	cmd_function_s* result = cmd_functions;

	if (!result)
	{
		return 0;
	}

	while (strcmp(cmdName, result->name))
	{
		result = result->next;
		if (!result)
		{
			return 0;
		}
	}

	return result;
}

//DONE : 0x470090
void Cmd_AddCommandInternal(const char* cmdName, void(__cdecl* function)(), cmd_function_s* allocedCmd, std::int32_t flags)
{
	if (Cmd_FindCommand(cmdName))
	{
		if (function)
		{
			Com_Printf(16, "Cmd_AddCommand: %s already defined\n", cmdName);
		}
	}
	else
	{
		allocedCmd->name = cmdName;
		allocedCmd->function = function;
		allocedCmd->flags = flags;
        allocedCmd->next = cmd_functions;
		cmd_functions = allocedCmd;
	}
}
