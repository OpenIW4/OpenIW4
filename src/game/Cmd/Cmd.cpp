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

//DONE : 0x4379F0
void Cmd_RemoveCommand(const char* cmdName)
{
    auto p_next = &cmd_functions;
    auto v2 = *cmd_functions;

    if (cmd_functions)
    {
        while (strcmp(cmdName, v2->name))
        {
            *p_next = &v2->next;
            v2 = v2->next;

            if (!v2)
            {
                return;
            }
        }

        **p_next = v2->next;
    }
}

//DONE : 0x40EDC0
void Cmd_SetAutoComplete(const char* cmdName, const char* dir, const char* ext)
{
    cmd_function_s* cmd = Cmd_FindCommand(cmdName);
    cmd->autoCompleteDir = dir;
    cmd->autoCompleteExt = ext;
}
