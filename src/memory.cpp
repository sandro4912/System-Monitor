#include "memory"

#include "../include/linux/l_memory.h"

namespace Sysmon{

std::shared_ptr<Memory> Memory::makeMemory() {
#ifdef __linux__
    Linux::Memory memory = Linux::Memory::createFromFile();
    return std::move(std::make_unique<Linux::Memory>(memory));
#elif _WIN32
#endif
}

}
