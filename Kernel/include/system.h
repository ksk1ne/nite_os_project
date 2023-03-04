#include <macros>

namespace System
{
    void Out(const char* fmt, ...);

    void Reboot();
    void Shutdown();
    void Halt();
    [[noreturn]] void Panic(const char *msg, ...);

    void EntryPoint();
} // namespace System
