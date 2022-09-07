#include "Cmd.hpp"
#include "../Com/Com.hpp"


//DONE : 0x4A5510
cmd_function_s* Cmd_FindCommand(const char* cmdName)
{
    cmd_function_s* cmd;

    for (cmd = *cmd_functions; cmd; cmd = cmd->next)
    {
        if (!strcmp(cmdName, cmd->name))
        {
            return cmd;
        }
    }

    return 0;
}

//DONE : 0x470090
void Cmd_AddCommandInternal(const char* cmdName, void(__cdecl* function)(), cmd_function_s* allocedCmd, bool isKey)
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
        allocedCmd->isKey = isKey;
        allocedCmd->next = *cmd_functions;
        *cmd_functions = allocedCmd;
    }
}
