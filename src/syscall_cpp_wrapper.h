#ifndef KSH_SYSCALL_CPP_WRAPPER_H
#define KSH_SYSCALL_CPP_WRAPPER_H

#include <string>
#include <vector>

void execv_cpp_wrapper(const std::string executable, const std::vector<std::string> &args);

#endif // KSH_SYSCALL_CPP_WRAPPER_H