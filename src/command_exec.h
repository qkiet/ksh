#ifndef KSH_COMMAND_EXEC_H
#define KSH_COMMAND_EXEC_H

#include <string>
#include <vector>

int execute_command(const std::string &command, bool &properly_quoted);

#endif // KSH_COMMAND_EXEC_H
